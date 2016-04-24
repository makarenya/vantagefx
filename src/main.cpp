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

	HttpContext session(io_service, ctx);

    GwtVantageFxBundle bundle;

	std::string url = "https://binaryoptions.vantagefx.com/app/index.html";

	std::map<std::string, std::string> keys;

	HttpRequest index(url);
	auto index_response = session.send(std::move(index));

	HttpResponse response;
    try {
        response = index_response.get();
    }
    catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		io_service.stop();
		worker.join();
        return 0;
    }

	auto rx = std::regex("var\\s+(\\w+)\\s*=\\s*'([^']+)'\\s*;");
	std::smatch m;
	auto it = response.body().begin();
	while (std::regex_search(it, response.body().end(), m, rx)) {
		auto key = std::string(m[1].first, m[1].second);
		auto value = std::string(m[2].first, m[2].second);
		keys[key] = value;
		it = m[0].second;
	}

	auto filename = fs::path(DATA_DIR) / "work" / "47_Full.txt";

	auto cookie_response = session.send(GwtCookieRequest(keys["serverCookie"])).get().body();

	auto lut_parser = makeResponseParser(session.send(std::move(GwtLutRequest())).get().body(), bundle);

	GwtObjectPtr lut;
	
	try {
		lut = lut_parser.parse(true);
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		lut_parser.back(10);
		lut_parser.print(std::cout, 9);
		std::cout << ">>> ";
		lut_parser.print(std::cout, 100);
	}


	auto id = GwtQuery(lut, "lutTypes/[name='InstrumentTypeSuper']/luts/[name='ShortTerm']/id").first();

	std::cout << id.toString() << std::endl;

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
		for (auto &id : GwtQuery(refresh, query)) {
			ids.push_back(id.value.toString());
		}

		std::sort(ids.begin(), ids.end());
		std::string last = "";
		for (auto it = ids.begin(); it != ids.end();) {
			if (*it == last) {
				it = ids.erase(it);
			}
			else {
				last = *it++;
			}
		}

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
