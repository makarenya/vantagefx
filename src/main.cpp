#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "model/MainModel.h"
#include "model/PersonModel.h"
#include "viewmodel/MainViewModel.h"
#include "api/ResponseParser.h"
#include <iostream>
#include <fstream>
#include <map>
#include <src/api/GwtRpcRequest.h>
#include <src/api/GwtParseContext.h>
#include <boost/shared_ptr.hpp>
#include <src/api/GwtParser.h>
#include <src/api/GwtType.h>
#include <src/api/GwtObject.h>
#include <src/api/GwtField.h>
//#include <boost/filesystem.hpp>
#include <src/api/GwtBundle.h>
#include "src/serialized/LutResolverInitPackage.h"


using namespace vantagefx::api;
using namespace vantagefx::serialized;

int main(int argc, char *argv[]) {

    for(auto i = 1; i <= 6; i++) {
        std::string num = boost::lexical_cast<std::string>(i);
        std::string path = DATA_DIR + std::string("request") + num + ".bin";
		std::cout << "processing " << path;
        std::ifstream file_stream(path, std::ios::in | std::ios::binary);
        auto content = std::string(std::istream_iterator<char>(file_stream), std::istream_iterator<char>());
        auto data = ParseResponse(content);
        if (data.version == 7) {
            GwtVantageFxBundle bundle;
            GwtParser parser(data.strings, data.data, bundle);
            try
            {
				//boost::filesystem::create_directories(DATA_DIR + num);
				auto result = parser.parse();
                std::ofstream fs(DATA_DIR + num + "/result.xml");
                bundle.printTables(DATA_DIR + num + "/");
                result->print(fs, GwtPrintStyle::Xml);
				parser.print(std::cout, 300);
            }
            catch(std::exception &ex)
            {
                std::cout << ex.what() << std::endl;
                parser.print(std::cout, 20);
                return 2;
            }
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
