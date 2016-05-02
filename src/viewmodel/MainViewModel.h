//
// Created by alexx on 09.03.2016.
//

#ifndef QT_SECOND_MAINVIEWMODEL_H
#define QT_SECOND_MAINVIEWMODEL_H

#include <QtCore>
#include "OptionsListModel.h"
#include "ComboBoxModel.h"
#include "Controller.h"

namespace vantagefx {
    namespace viewmodel {

        class MainViewModel : public QObject
        {
        Q_OBJECT

        public:

            Q_PROPERTY(QString mode READ mode NOTIFY modeChanged);
            Q_PROPERTY(OptionsListModel *options READ options NOTIFY optionsChanged)
            Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
            Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
            Q_PROPERTY(QString server READ server WRITE setServer NOTIFY serverChanged)
            Q_PROPERTY(ComboBoxModel *servers READ servers NOTIFY serversChanged)
            Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged)
            Q_PROPERTY(QString money READ money NOTIFY moneyChanged)

        public:
            explicit MainViewModel(Controller &controller);
            MainViewModel(const MainViewModel &rhs) = delete;
            MainViewModel &operator=(const MainViewModel &rhs) = delete;

            virtual ~MainViewModel();

            OptionsListModel *options() { return &_options; }

            Q_INVOKABLE void doLogin();
            Q_INVOKABLE void processLogin();
            Q_INVOKABLE void cancelLogin();

            const QString &mode() const;
            void setMode(const QString &mode);

            const QString &login() const;
            void setLogin(const QString &login);

            const QString &password() const;
            void setPassword(const QString &password);

            const QString &server() const;
            void setServer(const QString &server);

            ComboBoxModel *servers();

            const QString &fullName() const;
            void setFullName(const QString &fullName);

            const QString &money() const;
            void setMoney(int64_t money);

        public slots:

            void update();

        signals:

            void modeChanged(QString mode);

            void optionsChanged(const OptionsListModel *options);

            void loginChanged(const QString &login);

            void passwordChanged(const QString &password);

            void serverChanged(const QString &server);

            void serversChanged(const ComboBoxModel *servers);

            void fullNameChanged(const QString &fullName);

            void moneyChanged(const QString &money);

        private:

            OptionsListModel _options;
            QString _mode;
            QString _login;
            QString _password;
            QString _server;
            ComboBoxModel _servers;
            QString _fullName;
            QString _money;

            Controller &_controller;
            int _refreshTimeout;
        };


        inline const QString &MainViewModel::mode() const { return _mode; }

        inline const QString &MainViewModel::login() const { return _login; }

        inline const QString &MainViewModel::password() const { return _password; }

        inline const QString &MainViewModel::server() const { return _server; }

        inline ComboBoxModel *MainViewModel::servers() { return &_servers; }

        inline const QString &MainViewModel::fullName() const { return _fullName; }
    }
}


#endif //QT_SECOND_MAINVIEWMODEL_H
