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

void processEntry(fs::path dir, std::string name, FiddlerLogEntry entry, GwtVantageFxBundle &bundle)
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
    data = ParseResponse(entry.response());
    if (data.version == 7) {
        GwtParser parser(data.strings, data.data, bundle);
        try {
            auto result = parser.parse();
            auto response = document.createElement("response");
            result->xml(response);
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

int main(int argc, char *argv[]) 
{
	auto filename = fs::path(DATA_DIR) / "log_and_work" / "17_Full.txt";
    GwtVantageFxBundle bundle;
	if (argc == 2) {
		if (!fs::is_regular_file(argv[1])) {
			std::cout << "file " << argv[1] << " not found" << std::endl;
			return 1;
		}
		filename = argv[1];
	}
    if (filename.extension() == ".bin") {
        fs::ifstream file_stream(filename, std::ios::in | std::ios::binary);
	    auto content = std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
        processResponse(filename.parent_path(), filename.stem().string(), content, bundle);
    }
    else {
        auto entries = parseEntries(filename);
        auto i = 0;
        for(auto entry: entries) {
            i++;
            std::vector<std::string> paths;
            boost::split(paths, entry.url(), boost::is_any_of("/"));
            auto name = boost::lexical_cast<std::string>(i) + "_" + paths[paths.size() - 1];
            std::cout << "processing " << name << std::endl;
            processEntry(filename.parent_path(), name, entry, bundle);
        }
    }
	return 0;

	//auto data = api::ParseRequest("7|0|5|https://binaryoptions.vantagefx.com/app/Basic/|46123D47AC2C515447AEF3C2580787E2|com.optionfair.client.common.services.TradingService|getAssetIntradayTicks|J|1|2|3|4|3|5|5|5|b|VOYMBnE|VOYPdVk|");
    //std::cout << api::GwtRpcRequest(data);



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

    /*
    QList<QObject*> phonebook;
    phonebook.push_back(new PersonViewModel(alexey));
    phonebook.push_back(new PersonViewModel(vasya));
    phonebook.push_back(new PersonViewModel(inga));
    phonebook.push_back(new PersonViewModel(catherine));
    */

    engine.rootContext()->setContextProperty("root", &mainViewModel);
    //engine.rootContext()->setContextProperty("phonebook", QVariant::fromValue(phonebook));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
