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
			Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
            Q_PROPERTY(OptionsListModel *options READ options NOTIFY optionsChanged)
            Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
            Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
            Q_PROPERTY(QString server READ server WRITE setServer NOTIFY serverChanged)
            Q_PROPERTY(ComboBoxModel *servers READ servers NOTIFY serversChanged)

			Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loggedInChanged)
            Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged)
            Q_PROPERTY(QString money READ money NOTIFY moneyChanged)

			Q_PROPERTY(QString optionName READ optionName NOTIFY optionNameChanged)
			Q_PROPERTY(int optionReturn READ optionReturn NOTIFY optionReturnChanged)
			Q_PROPERTY(QString optionExpire READ optionExpire NOTIFY optionExpireChanged)

        public:
            explicit MainViewModel(Controller &controller);
            MainViewModel(const MainViewModel &rhs) = delete;
            MainViewModel &operator=(const MainViewModel &rhs) = delete;

            virtual ~MainViewModel();

            OptionsListModel *options() { return &_options; }

            Q_INVOKABLE void doLogin();
            Q_INVOKABLE void processLogin();
            Q_INVOKABLE void cancelLogin();
            Q_INVOKABLE void view(long long optionId);
			Q_INVOKABLE void buyHigh();
			Q_INVOKABLE void buyLow();

            const QString &mode() const;
            void setMode(const QString &mode);

			const QString &description() const;
			void setDescription(const QString &description);

            const QString &login() const;
            void setLogin(const QString &login);

            const QString &password() const;
            void setPassword(const QString &password);

            const QString &server() const;
            void setServer(const QString &server);

            ComboBoxModel *servers();

			bool loggedIn() const;
			void setLoggedIn(bool loggedIn);

            const QString &fullName() const;
            void setFullName(const QString &fullName);

            const QString &money() const;
            void setMoney(int64_t money);

            const model::GwtOptionModel &currentOption() const;
            void setCurrentOption(const model::GwtOptionModel &option);

			const QString &optionName() const;
			void setOptionName(const QString &optionName);

			int optionReturn() const;
			void setOptionReturn(int optionReturn);

			const QString &optionExpire() const;
			void setOptionExpire(const QString &optionExpire);


        public slots:

	        void update();

        signals:

            void modeChanged();

			void descriptionChanged();

            void optionsChanged();

            void loginChanged();

            void passwordChanged();

            void serverChanged();

            void serversChanged();

			void loggedInChanged();

            void fullNameChanged();

            void moneyChanged();

			void optionNameChanged();

			void optionReturnChanged();

			void optionExpireChanged();

        private:

			void makePurchases();
			
			OptionsListModel _options;
            QString _mode;
			QString _description;
            QString _login;
            QString _password;
            QString _server;
            ComboBoxModel _servers;

			bool _loggedIn;
            QString _fullName;
            QString _money;

			model::GwtOptionModel _currentOption;
			int64_t _optionId;
			QString _optionName;
			int _optionReturn;
			QString _optionExpire;

            Controller &_controller;
            int _refreshTimeout;
        };


        inline const QString &MainViewModel::mode() const { return _mode; }

		inline const QString &MainViewModel::description() const { return _description; }

        inline const QString &MainViewModel::login() const { return _login; }

        inline const QString &MainViewModel::password() const { return _password; }

        inline const QString &MainViewModel::server() const { return _server; }

        inline ComboBoxModel *MainViewModel::servers() { return &_servers; }

		inline bool MainViewModel::loggedIn() const { return _loggedIn; }

        inline const QString &MainViewModel::fullName() const { return _fullName; }

		inline const model::GwtOptionModel &MainViewModel::currentOption() const { return _currentOption; }

		inline const QString &MainViewModel::optionName() const { return _optionName; }

		inline int MainViewModel::optionReturn() const { return _optionReturn; }

		inline const QString &MainViewModel::optionExpire() const { return _optionExpire; }
    }
}


#endif //QT_SECOND_MAINVIEWMODEL_H
