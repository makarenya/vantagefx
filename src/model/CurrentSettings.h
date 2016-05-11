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

        private:

            QString _login;
            QString _password;
            QString _server;
        };
    }
}


#endif //VANTAGEFX_CURRENTSETTINGS_H
