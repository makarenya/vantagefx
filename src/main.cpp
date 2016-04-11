#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDomDocument>
#include "model/MainModel.h"
#include "model/PersonModel.h"
#include "viewmodel/MainViewModel.h"
#include <iostream>
#include <fstream>
#include <src/api/GwtRpcRequest.h>
#include <src/api/GwtParseContext.h>
#include <src/api/GwtParser.h>
#include <src/api/GwtType.h>
#include <src/api/GwtBundle.h>
#include "src/serialized/LutResolverInitPackage.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <src/api/FiddlerLogEntry.h>
#include <src/api/FiddlerLogParser.h>
#include <src/api/GwtRequestParser.h>
#include <src/api/GwtResponseParser.h>
#include <src/api/GwtIterator.h>
#include "src/http/HttpRequest.h"
#include "http/HttpContext.h"
#include <thread>
#include <regex>


using namespace vantagefx::api;
using namespace vantagefx::serialized;
namespace fs = boost::filesystem;

void processResponse(fs::path dir, std::string name, std::string content, GwtVantageFxBundle &bundle)
{
    auto data = ParseResponse(content);
    fs::create_directory(dir / "results");
    if (data.version == 7) {
        GwtParser parser(data.strings, data.data, bundle);
        try {
            fs::path report = dir / "results" / (name + "_response.xml");
            auto result = parser.parse();
            QDomDocument document;
            auto body = document.createElement("request");
            result->xml(body);
            document.appendChild(body);
            QFile fs(report.string().c_str());
            fs.open(QIODevice::ReadWrite | QIODevice::Truncate);
            fs.write(document.toByteArray());
            fs.close();
            parser.print(std::cout, 100);
        }
        catch (std::exception &ex) {
            std::cout << ex.what() << std::endl;
            parser.print(std::cout, 20);
        }
    }
}

GwtObjectPtr processEntry(fs::path dir, std::string name, FiddlerLogEntry entry, GwtVantageFxBundle &bundle)
{
    fs::create_directory(dir / "results");
    fs::path report = dir / "results" / (name + ".xml");
    QDomDocument document;
    auto body = document.createElement("entry");
    body.setAttribute("method", entry.method().c_str());
    body.setAttribute("url", entry.url().c_str());
    body.setAttribute("code", entry.code().c_str());

    auto data = ParseRequest(entry.request());
    if (data.version == 7) {
        GwtParser parser(data.strings, data.data, bundle);
        GwtTypePtr type = std::make_shared<GwtRequestType>();
        try {
            GwtObjectPtr result = std::make_shared<GwtObject>(type);
            type->parse(parser, result);
            auto request = document.createElement("request");
            result->xml(request);
            body.appendChild(request);
            parser.print(std::cout, 100);
        }
        catch (std::exception &ex) {
            std::cout << ex.what() << std::endl;
            parser.print(std::cout, 20);
        }
    }
	GwtObjectPtr result;
    data = ParseResponse(entry.response());
    if (data.version == 7) {
        GwtParser parser(data.strings, data.data, bundle);
        try {
            result = parser.parse();
            auto response = document.createElement("response");
            result->xml(response);
			
			/*
			GwtValue search(50);
			std::vector<std::string> found;
			result->find(search, found);
			auto value = result->get("verifications/[Account]");
			*/

            body.appendChild(response);
            parser.print(std::cout, 100);
        }
        catch (std::exception &ex) {
            std::cout << ex.what() << std::endl;
            parser.print(std::cout, 20);
        }
    }
    document.appendChild(body);
    QFile fs(report.string().c_str());
    fs.open(QIODevice::ReadWrite | QIODevice::Truncate);
    fs.write(document.toByteArray());
    fs.close();
	return result;
}

std::vector<FiddlerLogEntry> parseEntries(fs::path filename)
{
    std::vector<FiddlerLogEntry> result;
    if (!fs::exists(filename) || !fs::is_regular_file(filename)) return result;
    fs::ifstream file_stream(filename, std::ios::in | std::ios::binary);

    FiddlerLogEntry current;
    FiddlerLogParser parser;

    for (std::istreambuf_iterator<char> it(file_stream); it != std::istreambuf_iterator<char>(); ++it) {
        auto state = parser.advance(*it);
        if (state == FiddlerLogParser::Invalid) {
            std::cout << "error" << std::endl;
        }
        if (state == FiddlerLogParser::Complete) {
            result.push_back(parser.result());
        }
    }
    return result;
}

std::vector<std::string> usedBy(GwtObjectPtr object, std::vector<std::string> ids)
{
	std::vector<std::string> variants;
	std::vector<std::string> old;
	auto first = true;
	for (auto id : ids) {
		GwtValue search(id);
		std::vector<std::string> found;
		for (auto pair : GwtQuery(object, "**/" + id)) {
			found.push_back(pair.first);
		}
		for (auto pair : GwtQuery(object, "**/[.=" + id + "]")) {
			found.push_back(pair.first);
		}
		variants.clear();
		for (auto item : found) {
			std::vector<std::string> parts;
			boost::split(parts, item, boost::is_any_of("/"));
			for (auto i = 0; i < parts.size(); i++) {
				std::string value = "";
				for (auto j = 0; j < parts.size(); j++) {
					if (!value.empty()) value += "/";
					value += (i == j) ? "*" : parts[j];
				}
				if (first || std::find(old.begin(), old.end(), value) != old.end()) {
					if (std::find(variants.begin(), variants.end(), value) == variants.end()) {
						variants.push_back(value);
					}
				}
			}
		}
		std::swap(variants, old);
		first = false;
	}
	return variants;
}


int main(int argc, char *argv[]) 
{
	using namespace vantagefx::http;

	asio::io_service io_service;
	auto ctx = ssl::context(boost::asio::ssl::context::sslv23);
	ctx.load_verify_file(std::string(DATA_DIR) + "ca.cer");

	HttpContext session(io_service, ctx);

	std::string url = "https://binaryoptions.vantagefx.com/app/index.html";

	std::map<std::string, std::string> keys;

	session.get(url, [&keys](HttpRequestPtr request)
	{
		auto &response = request->response();
		auto rx = std::regex("var\\s+(\\w+)\\s*=\\s*'([^']+)'\\s*;");
		std::smatch m;
		auto it = response.body().begin();
		while (std::regex_search(it, response.body().end(), m, rx)) {
			auto key = std::string(m[1].first, m[1].second);
			auto value = std::string(m[2].first, m[2].second);
			keys[key] = value;
			it = m[0].second;
		}
	});

	while (keys.size() == 0) {
		io_service.poll();
	}

	auto filename = fs::path(DATA_DIR) / "work" / "47_Full.txt";

	auto cookie = std::make_shared<HttpRequest>("https://binaryoptions.vantagefx.com/app/services/cookie", "POST");
	cookie->addHeader("X-GWT-Module-Base", "https://binaryoptions.vantagefx.com/app/Basic/");
	cookie->addHeader("X-GWT-Permutation", "97A60A56BD971D60069316AF35BCDFB9");
	cookie->addHeader("Origin", "https://binaryoptions.vantagefx.com");
	cookie->addHeader("Content-Type", "text/x-gwt-rpc; charset=UTF-8");

	auto data = "7|0|6|https://binaryoptions.vantagefx.com/app/Basic/|F37CB27F20251B873A47EC6A32F293C7|"
		"com.optionfair.client.cookies.CookieSaver|saveCookie|java.lang.String/2004016611|" +
		keys["serverCookie"] + "|1|2|3|4|1|5|6|";

	cookie->setContent(data);

	std::string dd = "";

	cookie->setHandler([&dd](HttpRequestPtr request)
	{
		auto &response = request->response();
		dd = response.body();
	});

	session.send(cookie);

	while(dd.empty()) {
		io_service.poll();
	}


    GwtVantageFxBundle bundle;
	if (argc == 2) {
		if (!fs::is_regular_file(argv[1])) {
			std::cout << "file " << argv[1] << " not found" << std::endl;
			return 1;
		}
		filename = argv[1];
	}
	if (argc == 3 && std::string(argv[1]) == "find") {
		auto entries = parseEntries(filename);
        GwtResponseData data;
		data = ParseResponse(entries[1].response());
        auto table = GwtParser(data.strings, data.data, bundle).parse();
        data = ParseResponse(entries[16].response());
        auto refresh = GwtParser(data.strings, data.data, bundle).parse();

		std::vector<std::string> ids;
		std::string query = argv[2];
		for (auto id : GwtQuery(refresh, query)) {
			ids.push_back(id.second.toString());
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
		auto keys = usedBy(table, ids);

		for (auto key : keys) {
			std::cout << key << std::endl;
		}
		return 0;
	}
	if (argc > 3 && std::string(argv[1]) == "values") {
		auto entries = parseEntries(filename);
		GwtResponseData data;
		data = ParseResponse(entries[1].response());
		auto table = GwtParser(data.strings, data.data, bundle).parse();

		std::vector<std::string> ids;
		for (auto i = 2; i < argc; i++)
		{
			ids.push_back(argv[i]);
		}

		std::cout << std::endl << "paths:" << std::endl;
		auto keys = usedBy(table, ids);

		for (auto key : keys) {
			std::cout << key << std::endl;
		}
		return 0;
	}
	if (filename.extension() == ".bin") {
        fs::ifstream file_stream(filename, std::ios::in | std::ios::binary);
	    auto content = std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
        processResponse(filename.parent_path(), filename.stem().string(), content, bundle);
    }
    else {
        auto entries = parseEntries(filename);
        auto i = 0;
		std::vector<GwtObjectPtr> created;
        for(auto entry: entries) {
            std::vector<std::string> paths;
            boost::split(paths, entry.url(), boost::is_any_of("/"));
            auto name = boost::lexical_cast<std::string>(i) + "_" + paths[paths.size() - 1];
            std::cout << "processing " << name << std::endl;
            created.push_back(processEntry(filename.parent_path(), name, entry, bundle));
			i++;
		}
		fs::create_directory(filename.parent_path() / "tables");
		bundle.printTables(filename.parent_path() / "tables");
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
