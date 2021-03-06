//
// Created by alexx on 09.03.2016.
//

#ifndef QT_SECOND_MAINVIEWMODEL_H
#define QT_SECOND_MAINVIEWMODEL_H

#include <QtCore>
#include "AssetListModel.h"
#include "ComboBoxModel.h"
#include "VantageFxService.h"
#include "AssetsViewModel.h"
#include "../model/VantageFxModel.h"
#include "../model/CurrentSettings.h"
#include "../controller/StatisticRenderer.h"

namespace vantagefx {
    namespace viewmodel {

        class MainViewModel : public QObject
        {
        Q_OBJECT

        public:

            Q_PROPERTY(QString mode READ mode NOTIFY modeChanged);
			Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
            Q_PROPERTY(AssetListModel *options READ options NOTIFY optionsChanged)
			Q_PROPERTY(AssetsViewModel *assets READ assets NOTIFY assetsChanged)
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

			Q_PROPERTY(int betAmount READ betAmount NOTIFY betAmountChanged)
			Q_PROPERTY(int betsCount READ betsCount NOTIFY betsCountChanged)

        public:
            explicit MainViewModel(VantageFxService &servie);
            MainViewModel(const MainViewModel &rhs) = delete;
            MainViewModel &operator=(const MainViewModel &rhs) = delete;

            virtual ~MainViewModel();

            Q_INVOKABLE void doLogin();
            Q_INVOKABLE void processLogin();
            Q_INVOKABLE void cancelLogin();
            Q_INVOKABLE void view(long long optionId);
			Q_INVOKABLE void setBet(int betAmount, int betsCount);
            Q_INVOKABLE void watch(int index);
            Q_INVOKABLE void stopWatch(int assetId);

            AssetListModel *options();
            AssetsViewModel *assets();

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

            const model::OptionModel &currentOption() const;
            void setCurrentOption(const model::OptionModel &option);

			const QString &optionName() const;
			void setOptionName(const QString &optionName);

			int optionReturn() const;
			void setOptionReturn(int optionReturn);

			const QString &optionExpire() const;
			void setOptionExpire(const QString &optionExpire);

            int betAmount() const;

            int betsCount() const;

        public slots:

	        void update();

        signals:

            void modeChanged();

			void descriptionChanged();

            void optionsChanged();

			void assetsChanged();

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

			void betAmountChanged();

			void betsCountChanged();

		public:

			void doLoad();
			Q_SIGNAL void loadedSignal(LoadingContextPtr ctx);
			Q_SIGNAL void loadingErrorSignal(std::exception e);
			Q_SLOT void loaded(LoadingContextPtr ctx);
			Q_SLOT void loadingError(std::exception e);

			void doRefresh();
			Q_SIGNAL void refreshedSignal(RefreshContextPtr ctx);
			Q_SIGNAL void refreshErrorSignal(std::exception e);
			Q_SLOT void refreshed(RefreshContextPtr ctx);
			Q_SLOT void refreshError(std::exception e);

			void doAuth(QString login, QString password, QString server);
			Q_SIGNAL void authenticatedSignal(AuthContextPtr ctx);
			Q_SIGNAL void authErrorSignal(std::exception e);
			Q_SLOT void authenticated(AuthContextPtr ctx);
			Q_SLOT void authError(std::exception e);

			void doPurchase(int64_t optionId, int64_t money, int positionType);
			Q_SIGNAL void purchasedSignal(PurchaseContextPtr ctx);
			Q_SIGNAL void purchaseErrorSignal(std::exception e);
			Q_SLOT void purchased(PurchaseContextPtr ctx);
			Q_SLOT void purchaseError(std::exception e);


			bool makePurchases(const QMap<int64_t, model::OptionModel> &options);
			
			model::CurrentSettings _settings;
			AssetListModel _options;
			AssetsViewModel _assets;
            QString _mode;
			QString _description;
            QString _login;
            QString _password;
            QString _server;
            ComboBoxModel _servers;

			bool _loggedIn;
            QString _fullName;
            QString _money;

			model::OptionModel _currentOption;
			QString _optionName;
			int _optionReturn;
			QString _optionExpire;
			controller::StatisticRenderer _stat;

			VantageFxService &_service;
			model::VantageFxModel _model;
            int _refreshTimeout;
			int _lastHour;
            int _lastDay;
        };

        inline AssetListModel *MainViewModel::options() { return &_options; }
        inline AssetsViewModel *MainViewModel::assets() { return &_assets; }

        inline const QString &MainViewModel::mode() const { return _mode; }

		inline const QString &MainViewModel::description() const { return _description; }

        inline const QString &MainViewModel::login() const { return _login; }

        inline const QString &MainViewModel::password() const { return _password; }

        inline const QString &MainViewModel::server() const { return _server; }

        inline ComboBoxModel *MainViewModel::servers() { return &_servers; }

		inline bool MainViewModel::loggedIn() const { return _loggedIn; }

        inline const QString &MainViewModel::fullName() const { return _fullName; }

		inline const model::OptionModel &MainViewModel::currentOption() const { return _currentOption; }

		inline const QString &MainViewModel::optionName() const { return _optionName; }

		inline int MainViewModel::optionReturn() const { return _optionReturn; }

		inline const QString &MainViewModel::optionExpire() const { return _optionExpire; }

        inline int MainViewModel::betAmount() const { return _model.betAmount(); }

        inline int MainViewModel::betsCount() const { return _model.betsCount(); }
    }
}


#endif //QT_SECOND_MAINVIEWMODEL_H
