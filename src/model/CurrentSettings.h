//
// Created by AMakarenya on 12.05.2016.
//

#ifndef VANTAGEFX_CURRENTSETTINGS_H
#define VANTAGEFX_CURRENTSETTINGS_H

#include <QtCore>

namespace vantagefx {
    namespace model {
        class CurrentSettings {
        public:

            CurrentSettings();
            void load();
            void save() const;

            const QString &login() const;
            void setLogin(const QString &login);

            const QString &password() const;
            void setPassword(const QString &password);

            const QString &server() const;
            void setServer(const QString &server);

			const QSet<int> &assets() const;
			void appendAsset(int assetId);
			void removeAsset(int assetId);

        private:

            QString _login;
            QString _password;
            QString _server;
			QSet<int> _assets;
        };
    }
}


#endif //VANTAGEFX_CURRENTSETTINGS_H
