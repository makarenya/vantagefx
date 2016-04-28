#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <src/api/GwtRpcRequest.h>
#include <src/api/GwtType.h>
#include <src/api/GwtBundle.h>
#include "src/serialized/LutResolverInitPackage.h"
#include "src/http/HttpRequest.h"
#include "src/analyzer/GwtAnalyzer.h"
#include <thread>
#include <boost/locale.hpp>
#include <src/viewmodel/MainViewModel.h>
#include "src/viewmodel/Controller.h"
#ifdef WINVER
#include <shellapi.h>
#endif

using namespace vantagefx::api;
using namespace vantagefx::serialized;
namespace fs = boost::filesystem;

int start(int argc, char **argv, fs::path module_dir);

#ifdef WINVER
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

	auto paths = QCoreApplication::libraryPaths();
	paths.append(".");
	paths.append("imageformats");
	paths.append("platforms");
	paths.append("sqldrivers");
	QCoreApplication::setLibraryPaths(paths);


	return start(argc, &args[0], module.parent_path() / "ca.cer");
}
#else
int main(int argc, char** argv)
{
	return start(argc, argv, fs::path(DATA_DIR) / "ca.cer");
}
#endif

int start(int argc, char **argv, fs::path ca_path)
{
	using namespace vantagefx;
	using namespace vantagefx::http;
    using namespace vantagefx::analyzer;
    using namespace vantagefx::viewmodel;

	asio::io_service io_service;
	asio::io_service::work work(io_service);

	std::thread worker([&io_service]() {
		io_service.run();
	});

	auto ctx = ssl::context(ssl::context::sslv23_client);
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

	engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");

	GwtVantageFxBundle bundle;
	MainViewModel mainViewModel(Controller(GwtHttpContext(io_service, ctx, bundle)));

    engine.rootContext()->setContextProperty("root", &mainViewModel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    auto code = app.exec();
    io_service.stop();
    worker.join();
    return code;
}
