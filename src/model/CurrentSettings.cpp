//
// Created by AMakarenya on 12.05.2016.
//

#include "CurrentSettings.h"
#include <QDomDocument>

namespace vantagefx {
    namespace model {

        CurrentSettings::CurrentSettings()
        {
        }

        void CurrentSettings::load()
        {
            auto appData = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
			if (!appData.exists()) return;
            auto settingsPath = appData.filePath("settings.xml");
            if (QFile::exists(settingsPath)) {
                QDomDocument doc;
                QFile file(settingsPath);
                if (file.open(QIODevice::ReadOnly) && doc.setContent(&file)) {
                    auto root = doc.documentElement();

                    auto login = root.namedItem("login");
                    if (login.isElement()) _login = login.toElement().text();

                    auto password = root.namedItem("password");
                    if (password.isElement()) _password = password.toElement().text();

                    auto server = root.namedItem("server");
                    if (server.isElement()) _server = server.toElement().text();

					auto assets = root.namedItem("assets");
					_assets.clear();
					if (assets.isElement()) {
						auto nodes = assets.toElement().childNodes();
						for(auto i = 0; i < nodes.size(); ++i) {
							if (nodes.at(i).isElement()) {
								_assets.insert(nodes.at(i).toElement().text().toInt());
							}
						}
					}
                }
            }
        }

        void CurrentSettings::save() const
        {
            auto appData = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
			appData.mkpath(appData.path());
			auto settingsPath = appData.filePath("settings.xml");
            QDomDocument doc;
            auto root = doc.createElement("root");
            doc.appendChild(root);

            auto login = doc.createElement("login");
			login.appendChild(doc.createTextNode(_login));
            root.appendChild(login);

            auto password = doc.createElement("password");
            password.appendChild(doc.createTextNode(_password));
            root.appendChild(password);

            auto server = doc.createElement("server");
            server.appendChild(doc.createTextNode(_server));
            root.appendChild(server);

			auto assets = doc.createElement("assets");
			for (auto item : _assets) {
				auto node = doc.createElement("item");
				node.appendChild(doc.createTextNode(QString::number(item)));
				assets.appendChild(node);
			}
			root.appendChild(assets);

            QFile file(settingsPath);
            file.open(QIODevice::ReadWrite | QIODevice::Truncate);
            file.write(doc.toByteArray());
            file.close();
        }

        void CurrentSettings::setLogin(const QString &login)
        {
            _login = login;
        }

        const QString &CurrentSettings::login() const
        {
            return _login;
        }

        void CurrentSettings::setPassword(const QString &password)
        {
            _password = password;
        }

        const QString &CurrentSettings::password() const
        {
            return _password;
        }

        void CurrentSettings::setServer(const QString &server)
        {
            _server = server;
        }

        const QString &CurrentSettings::server() const
        {
            return _server;
        }

        const QSet<int> &CurrentSettings::assets() const
		{
			return _assets;
		}

		void CurrentSettings::appendAsset(int assetId)
		{
			_assets.insert(assetId);
		}

		void CurrentSettings::removeAsset(int assetId)
		{
			_assets.remove(assetId);
		}
    }
}

