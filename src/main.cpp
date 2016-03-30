#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDomDocument>
#include "model/MainModel.h"
#include "model/PersonModel.h"
#include "viewmodel/MainViewModel.h"
#include "api/ResponseParser.h"
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


using namespace vantagefx::api;
using namespace vantagefx::serialized;
namespace fs = boost::filesystem;

void processFile(fs::path filename) {
    if (!fs::exists(filename) || !fs::is_regular_file(filename)) {
        std::cout << filename << " not exists" << std::endl;
        return;
    }
    std::cout << "processing " << filename << std::endl;
    fs::ifstream file_stream(filename, std::ios::in | std::ios::binary);
    auto content = std::string(std::istream_iterator<char>(file_stream), std::istream_iterator<char>());
    auto data = ParseResponse(content);
    auto dir = filename.parent_path();
    fs::create_directory(dir / "tables");
    fs::create_directory(dir / "results");
    if (data.version == 7) {
        GwtVantageFxBundle bundle;
        GwtParser parser(data.strings, data.data, bundle);
        try {
            auto tables = dir / "tables" / filename.stem();
            auto report = dir / "results" / fs::change_extension(filename.filename(), ".xml");

            fs::create_directory(tables);

            auto result = parser.parse();
            bundle.printTables(tables);
            result->save(report);
            parser.print(std::cout, 100);
        }
        catch (std::exception &ex) {
            std::cout << ex.what() << std::endl;
            parser.print(std::cout, 20);
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc == 2) {
        processFile(argv[1]);
    }
    else {
        fs::path current = DATA_DIR;// fs::current_path() / "data";
        for (fs::path path: fs::directory_iterator(current)) {
            if (path.extension() == ".bin")
                processFile(path);
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
