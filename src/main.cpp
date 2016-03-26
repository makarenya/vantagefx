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
#include "src/serialized/LutResolverInitPackage.h"


using namespace vantagefx::api;
using namespace vantagefx::serialized;

int main(int argc, char *argv[])
{
    std::ifstream file_stream("C:\\Users\\AMakarenya\\OneDrive\\projects\\qt_second\\request.bin", std::ios::in | std::ios::binary);

	auto content = std::string(std::istream_iterator<char>(file_stream), std::istream_iterator<char>());

    auto data = ParseResponse(content);

    if (data.version == 7) {
        GwtParseContext ctx(data.strings, data.data);

        LutResolverInitPackage package;
        try {
            ctx >> checked(package);
        }
        catch(std::exception &ex) {
			std::cout << ex.what() << std::endl;
			return 2;
        }

		//std::cout << "<package>" << package << "</package>";

		std::cerr << std::endl;

		ctx.print(std::cerr, 200);

		std::cerr << "elements lost: " << ctx.count();

        /*
        for(int i = 0; i < 50; i++) {
            boost::apply_visitor(api::PrintVisitor(std::cout, data.strings), *(--last)) << std::endl;
        }
         */


        return 0;
    }
    return 1;



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
