#include <QApplication>
#include <QQmlApplicationEngine>
#include <iostream>
#include <src/api/GwtRpcRequest.h>
#include <src/api/GwtParseContext.h>
#include <src/api/GwtParser.h>
#include <src/api/GwtType.h>
#include <src/api/GwtBundle.h>
#include "src/serialized/LutResolverInitPackage.h"
#include <boost/algorithm/string.hpp>
#include <src/api/GwtResponseParser.h>
#include <src/api/GwtIterator.h>
#include "src/http/HttpRequest.h"
#include "src/analyzer/GwtAnalyzer.h"
#include "GwtHttpRequest.h"
#include "http/HttpContext.h"
#include <thread>
#include <regex>
#include <boost/locale.hpp>
#include <boost/filesystem/fstream.hpp>


using namespace vantagefx::api;
using namespace vantagefx::serialized;
namespace fs = boost::filesystem;

struct GwtOptionModel
{
	int optionId;
	std::string name;
	int moneyBack;
	int seconds;
	int rateHi;
	double price;
};

namespace vantagefx {

	using std::placeholders::_1;
	using std::placeholders::_2;

	class Controller
	{
	public:
		explicit Controller(GwtHttpContext &&context)
			: _context(std::move(context)),
			  _instrumentTypeId(0),
			  _brandId(0)
		{}

		void load(std::promise<std::map<int, GwtOptionModel> &> &&promise)
		{
			using namespace api;
			using namespace http;

			_promise = std::move(promise);
			std::string url = "https://binaryoptions.vantagefx.com/app/index.html";

			std::map<std::string, std::string> keys;

			HttpRequest index(url);
			_context.send(std::move(index), std::bind(&Controller::indexLoaded, this, _1, _2));
		}

		void refresh(std::promise<std::map<int, GwtOptionModel> &> &&promise)
		{
			_promise = std::move(promise);
			_context.gwt(GwtCometUpdatesRequest(_instrumentTypeId),
				std::bind(&Controller::refreshLoaded, this, _1, _2));
		}

		void save()
		{
			_lut->saveXml(fs::path(DATA_DIR) / "lut.xml");
			_instrumentConfiguration->saveXml(fs::path(DATA_DIR) / "instrumentConfiguration.xml");
			_instrumentOptions->saveXml(fs::path(DATA_DIR) / "instrumentOptions.xml");
			_refresh->saveXml(fs::path(DATA_DIR) / "refresh.xml");
			_auth->saveXml(fs::path(DATA_DIR) / "auth.xml");
		}

	private:

		bool handleError(const boost::optional<std::exception> &e)
		{
			if (!e) return false;
			_e = e;
			_promise.set_exception(std::make_exception_ptr(*_e));
			return true;
		}

		bool handleError(const boost::system::error_code &ec)
		{
			if (!ec) return false;
			_e = boost::system::system_error(ec);
			_promise.set_exception(std::make_exception_ptr(*_e));
			return true;
		}

		void indexLoaded(http::HttpResponse && response, const boost::system::error_code &ec)
		{
			if (handleError(ec)) return;

			auto rx = std::regex("var\\s+(\\w+)\\s*=\\s*'([^']+)'\\s*;");
			std::smatch m;
			auto it = response.body().begin();
			while (std::regex_search(it, response.body().end(), m, rx)) {
				auto key = std::string(m[1].first, m[1].second);
				auto value = std::string(m[2].first, m[2].second);
				_keys[key] = value;
				it = m[0].second;
			}

			_context.gwt(GwtCookieRequest(_keys["serverCookie"]), 
				std::bind(&Controller::cookieSets, this, _1, _2));
		}

		void cookieSets(GwtObjectPtr &&cookie, const boost::optional<std::exception> &e)
		{
			if (handleError(e)) return;
			_context.gwt(GwtLutRequest(), 
				std::bind(&Controller::lutLoaded, this, _1, _2));
		}

		void lutLoaded(GwtObjectPtr &&lut, const boost::optional<std::exception> &e)
		{
			if (handleError(e)) return;
			_lut = std::move(lut);

			auto instrumentTypeSuper = _lut->item("lutTypes/[name='InstrumentTypeSuper']/luts/[name='ShortTerm']/id");
			_instrumentTypeId = _lut->item("superRels/[instrumentTypeSuperId={0}]/instrumentTypeId",
				{ instrumentTypeSuper }).toInt();

			_brandId = _lut->value("externalId").toInt();

			_above = _lut->item("lutTypes/[name='PositionType']/luts/[name='Put']/id");

			_context.gwt(GwtInstrumentConfigurationDataRequest(_brandId), 
				std::bind(&Controller::instrumentConfigurationLoaded, this, _1, _2));
		}

		void instrumentConfigurationLoaded(GwtObjectPtr &&instrumentConfiguration, const boost::optional<std::exception> &e)
		{
			if (handleError(e)) return;
			_instrumentConfiguration = std::move(instrumentConfiguration);
			_context.gwt(GwtInstrumentTypeIdsWithOpenOptionsRequest(),
				std::bind(&Controller::instrumentOptionsLoaded, this, _1, _2));
		}

		void instrumentOptionsLoaded(GwtObjectPtr &&instrumentOptions, const boost::optional<std::exception> &e)
		{
			if (handleError(e)) return;
			_instrumentOptions = std::move(instrumentOptions);
			_context.gwt(GwtAuthRequest("26251", "3361e6c1147f89e", "live"),
				std::bind(&Controller::loggedIn, this, _1, _2));
		}

		void loggedIn(GwtObjectPtr &&auth, const boost::optional<std::exception> &e)
		{
			if (handleError(e)) return;
			_auth = std::move(auth);
			refresh(std::move(_promise));
		}

		void refreshLoaded(GwtObjectPtr &&data, const boost::optional<std::exception> &e)
		{
			if (handleError(e)) return;
			_refresh = data;
			for (auto &opt : data->query("options/*")) {

				auto obj = opt.value.toObject();
				auto id = obj->value("id").toInt();
				auto &result = _options[id];

				result.moneyBack = obj->value("return").toInt();
				result.seconds = obj->value("optionSeconds").toInt();
				auto asset = obj->value("assetId");
				result.name = _lut->item("assets/[id={0}]/some_34", { asset }).toString();
				result.price = data->item("assetUpdates/[assetId={0}]/targetPrice", { asset }).toDouble();
				result.rateHi = data->item("positionsSentimentDto/map/[int()={0}]/[int()={1}]/value", { asset, _above }).toInt();
			}
			_promise.set_value(_options);
		}

	private:
		GwtHttpContext _context;
		GwtObjectPtr _lut;
		int _instrumentTypeId;
		int _brandId;
		GwtValue _above;
		GwtObjectPtr _instrumentConfiguration;
		GwtObjectPtr _instrumentOptions;
		GwtObjectPtr _refresh;
		GwtObjectPtr _auth;
		std::map<std::string, std::string> _keys;
		std::map<int, GwtOptionModel> _options;
		boost::optional<std::exception> _e;
		std::promise<std::map<int, GwtOptionModel> &> _promise;
	};

}


int main(int argc, char *argv[]) 
{
	using namespace vantagefx;
	using namespace vantagefx::http;
    using namespace vantagefx::analyzer;

	asio::io_service io_service;
	asio::io_service::work work(io_service);

	std::thread worker([&io_service]() {
		io_service.run();
		std::cout << "stop" << std::endl;
	});

	auto ctx = ssl::context(ssl::context::sslv23_client);
	SSL_CTX_set_cipher_list(ctx.native_handle(), "TLSv1:SSLv3:SSLv2");


	ctx.load_verify_file(std::string(DATA_DIR) + "ca.cer");
	ctx.set_options(ssl::context::default_workarounds);

	GwtVantageFxBundle bundle;
	;


	auto filename = fs::path(DATA_DIR) / "work" / "47_Full.txt";

	Controller ctr(GwtHttpContext(io_service, ctx, bundle));

	std::promise<std::map<int, GwtOptionModel> &> promise;
	auto future = promise.get_future();
	ctr.load(std::move(promise));

	auto options = future.get();
	ctr.save();
	std::set<std::string> names;
	for(auto &pair : options) {
		if (names.find(pair.second.name) == names.end()) {
			std::cout << pair.second.name << " = " << pair.second.rateHi << "/" << (100 - pair.second.rateHi) << std::endl;
			names.insert(pair.second.name);
		}
	}

	/*
	GwtAnalyzer analyzer(bundle);


	std::vector<std::string> ids;
	for(auto &itemId: refresh->query("positionsSentimentDto/map/ * /key()")) {
		ids.push_back(itemId.value.toString());
	}

	std::cout << std::endl << "paths:" << std::endl;
	auto foundKeys = analyzer.usedBy(lut, ids);

	for (auto key : foundKeys) {
		std::cout << key << std::endl;
	}
	*/

	io_service.stop();
	worker.join();
	return 0;


	GwtAnalyzer analyzer(bundle);
	if (argc == 2) {
		if (!fs::is_regular_file(argv[1])) {
			std::cout << "file " << argv[1] << " not found" << std::endl;
			return 1;
		}
		filename = argv[1];
	}
	if (argc == 3 && std::string(argv[1]) == "find") {
		auto entries = analyzer.parseEntries(filename);
		auto table = makeResponseParser(entries[1].response(), bundle).parse();
		auto refresh = makeResponseParser(entries[16].response(), bundle).parse();

		std::vector<std::string> ids;
		std::string query = argv[2];
		for (auto &id : refresh->query(query)) {
			ids.push_back(id.value.toString());
		}

		/*
		std::sort(ids.begin(), ids.end());
		GwtValue last;
		for (auto it = ids.begin(); it != ids.end();) {
			if (*it == last) {
				it = ids.erase(it);
			}
			else {
				last = *it++;
			}
		}
		*/

		std::cout << "found id:" << std::endl;
		for (auto key : ids) {
			std::cout << key << std::endl;
		}

		std::cout << std::endl << "paths:" << std::endl;
		auto keys = analyzer.usedBy(table, ids);

		for (auto key : keys) {
			std::cout << key << std::endl;
		}
		return 0;
	}
	if (argc > 3 && std::string(argv[1]) == "values") {
		auto entries = analyzer.parseEntries(filename);
		auto table = makeResponseParser(entries[1].response(), bundle).parse();

		std::vector<std::string> ids;
		for (auto i = 2; i < argc; i++)
		{
			ids.push_back(argv[i]);
		}

		std::cout << std::endl << "paths:" << std::endl;
		auto keys = analyzer.usedBy(table, ids);

		for (auto key : keys) {
			std::cout << key << std::endl;
		}
		return 0;
	}
	if (filename.extension() == ".bin") {
        fs::ifstream file_stream(filename, std::ios::in | std::ios::binary);
	    auto content = std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
        analyzer.processResponse(filename.parent_path(), filename.stem().string(), content);
    }
    else {
        auto entries = analyzer.parseEntries(filename);
        auto i = 0;
		std::vector<GwtObjectPtr> created;
        for(auto entry: entries) {
            std::vector<std::string> paths;
			boost::split(paths, entry.url(), [](char c) { return c == '/'; });
            auto name = boost::lexical_cast<std::string>(i) + "_" + paths[paths.size() - 1];
            std::cout << "processing " << name << std::endl;
            created.push_back(analyzer.processEntry(filename.parent_path(), name, entry));
			i++;
		}
		fs::create_directory(filename.parent_path() / "tables");
		//bundle.printTables(filename.parent_path() / "tables");
    }
	return 0;

	//auto data = api::ParseRequest("7|0|5|https://binaryoptions.vantagefx.com/app/Basic/|46123D47AC2C515447AEF3C2580787E2|com.optionfair.client.common.services.TradingService|getAssetIntradayTicks|J|1|2|3|4|3|5|5|5|b|VOYMBnE|VOYPdVk|");
    //std::cout << api::GwtRpcRequest(data);


/*
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    MainModel mainModel;
    MainViewModel mainViewModel(mainModel);

    auto alexey = PersonModel()
            .setName("Alexey")
            .setSurname("Makarenya")
            .setPhone("8 (926) 288 0000")
            .setAge(32);

    auto vasya = PersonModel()
            .setName("Vasya")
            .setSurname("Pupkin")
            .setPhone("8 (905) 111 0000")
            .setAge(24);

    auto inga = PersonModel()
            .setName("Inga")
            .setSurname("Knish")
            .setPhone("8 (499) 777 44 33")
            .setAge(29);

    auto catherine = PersonModel()
            .setName("Catherine")
            .setSurname("Vdovina")
            .setPhone("8 (926) 548 5723")
            .setAge(27);


    mainViewModel.addPerson(alexey);
    mainViewModel.addPerson(vasya);
    mainViewModel.addPerson(inga);
    mainViewModel.addPerson(catherine);

    QList<QObject*> phonebook;
    phonebook.push_back(new PersonViewModel(alexey));
    phonebook.push_back(new PersonViewModel(vasya));
    phonebook.push_back(new PersonViewModel(inga));
    phonebook.push_back(new PersonViewModel(catherine));

    engine.rootContext()->setContextProperty("root", &mainViewModel);
    //engine.rootContext()->setContextProperty("phonebook", QVariant::fromValue(phonebook));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
    */
}
