#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <thread>
#include <boost/filesystem/fstream.hpp>
#include <boost/locale.hpp>
#include "src/api/GwtRpcRequest.h"
#include "src/api/GwtType.h"
#include "src/api/GwtBundle.h"
#include "src/serialized/LutResolverInitPackage.h"
#include "src/http/HttpRequest.h"
#include "src/analyzer/GwtAnalyzer.h"
#include "src/viewmodel/MainViewModel.h"
#include "src/viewmodel/VantageFxService.h"

#ifdef WINVER
#include <shellapi.h>
#endif

using namespace vantagefx::api;
using namespace vantagefx::serialized;
namespace fs = boost::filesystem;

int start(int argc, char **argv, fs::path module_dir);

#ifdef GUI_MAIN
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t *lpCmdLine, int nCmdShow)
{
	std::vector<wchar_t> buf(MAX_PATH);
	GetModuleFileNameW(hInstance, &buf[0], static_cast<int>(buf.size()));
	fs::path module = std::wstring(buf.begin(), buf.end());

    int argc;
	auto argv = CommandLineToArgvW(GetCommandLine(), &argc);
	std::vector<std::vector<char>> items(argc);
	std::vector<char*> args(argc);
	for(auto i = 0; i < argc; ++i) {
		auto arg = boost::locale::conv::utf_to_utf<char>(argv[i]);;
		items[i] = std::vector<char>(arg.begin(), arg.end());
		items[i].push_back(0);
		args[i] = &items[i][0];
	}
	LocalFree(argv);

	return start(argc, &args[0], module.parent_path() / "ca.cer");
}
#else
int main(int argc, char** argv)
{
	return start(argc, argv, fs::path(DATA_DIR) / "ca.cer");
}
#endif

int analyze(int argc, char **argv);

#ifdef USE_STATIC_QT
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QtQuick2Plugin)
Q_IMPORT_PLUGIN(QtQuick2WindowPlugin)
Q_IMPORT_PLUGIN(QtQuickControlsPlugin)
Q_IMPORT_PLUGIN(QtQuickLayoutsPlugin)
#endif

int start(int argc, char **argv, fs::path ca_path)
{
	using namespace vantagefx;
	using namespace vantagefx::http;
    using namespace vantagefx::analyzer;
    using namespace vantagefx::viewmodel;

	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
	if (argc >= 2 && (argv[1] == std::string("analyze") || argv[1] == std::string("find")
		|| argv[1] == std::string("values") || argv[1] == std::string("table"))) {
		return analyze(argc, argv);
	}

	asio::io_service io_service;
	asio::io_service::work work(io_service);

	std::thread worker([&io_service]() {
		io_service.run();
	});

	auto ctx = ssl::context(ssl::context::tlsv1_client);
	SSL_CTX_set_cipher_list(ctx.native_handle(), "TLSv1:SSLv3:SSLv2");

	auto cert_path = ca_path;

	ctx.load_verify_file(cert_path.string());
	ctx.set_options(ssl::context::default_workarounds);


	QApplication app(argc, argv);

	auto paths = QCoreApplication::libraryPaths();
	paths.append(".");
	paths.append("platforms");
	QCoreApplication::setLibraryPaths(paths);
	
	QQmlApplicationEngine engine;

	GwtVantageFxBundle bundle;
	VantageFxService controller(GwtHttpContext(io_service, ctx, bundle));
	MainViewModel mainViewModel(controller);

    engine.rootContext()->setContextProperty("root", &mainViewModel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    auto code = app.exec();
    controller.stop();
    io_service.stop();
    worker.join();
    return code;
}


int analyze(int argc, char **argv)
{
	using namespace vantagefx;
	using namespace vantagefx::analyzer;

	fs::path filename;
	GwtVantageFxBundle bundle;
	GwtAnalyzer analyzer(bundle);

	if (argc > 2) {
		if (!fs::is_regular_file(argv[2])) {
			std::cout << "file " << argv[1] << " not found" << std::endl;
			return 1;
		}
		filename = argv[2];
	}
	if (argc == 6 && std::string(argv[1]) == "find") {
		// find FIDDLER_LOG_PATH VALUES_INDEX SEARCH_INDEX QUERY
		auto entries = analyzer.parseEntries(filename);
		auto valuesIndex = boost::lexical_cast<int>(argv[3]);
		auto searchIndex = boost::lexical_cast<int>(argv[4]);

		auto table = makeResponseParser(entries[searchIndex].response(), bundle).parse();
		auto values = makeResponseParser(entries[valuesIndex].response(), bundle).parse();

		std::vector<std::string> ids;
		for (auto &id : values->query(argv[5])) {
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
	if (argc > 4 && std::string(argv[1]) == "values") {
		// analyze FIDDLER_LOG_PATH LOG_INDEX VALUE1 [VALUE2 [VALUE3...]]
		// Поиск путей, содержащих все указанные опции
		auto entries = analyzer.parseEntries(filename);
		auto index = boost::lexical_cast<int>(argv[3]);
		auto table = makeResponseParser(entries[index].response(), bundle).parse();

		std::vector<std::string> ids;
		for (auto i = 4; i < argc; i++)
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
	if (argc == 3 && std::string(argv[1]) == "analyze") {
		// analyze FIDDLER_LOG_PATH
		auto entries = analyzer.parseEntries(filename);
		auto i = 0;
		std::vector<GwtObjectPtr> created;
		for (auto entry : entries) {
			std::vector<std::string> paths;
			boost::split(paths, entry.url(), [](char c) { return c == '/'; });
			auto name = boost::lexical_cast<std::string>(i) + "_" + paths[paths.size() - 1];
			std::cout << "processing " << name << std::endl;
			created.push_back(analyzer.processEntry(filename.parent_path(), name, entry));
			i++;
		}
		return 0;
	}
	if (argc == 6 && std::string(argv[1]) == "table") {
		// table FIDDLER_LOG_PATH LOG_INDEX QUERY CSV_PATH
		auto entries = analyzer.parseEntries(filename);
		auto index = boost::lexical_cast<int>(argv[3]);
		auto entry = entries[index];
		auto root = makeResponseParser(entry.response(), bundle).parse();
		std::string typeName = "";
		fs::ofstream file(fs::path(argv[5]), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
		for (auto item : root->query(argv[4])) {
			auto object = item.value.objectValue();
			if (!object) continue;
			if (typeName.empty()) {
				typeName = object->type()->name();
				auto first = true;
				for (auto it = object->iterateValues();!it->empty(); it->advance()) {
					if (first) first = false;
					else file << ";";
					file << it->field()->name();
				}
				file << std::endl;
			}
			if (typeName == object->type()->name()) {
				auto first = true;
				for (auto it = object->iterateValues();!it->empty(); it->advance()) {
					if (first) first = false;
					else file << ";";
					it->field()->print(it->get(), file, GwtPrintStyle::CsvValue);
				}
				file << std::endl;
			}
		}
		return 0;
	}
	return -1;
}