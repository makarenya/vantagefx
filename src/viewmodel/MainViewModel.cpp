//
// Created by alexx on 09.03.2016.
//
#include "MainViewModel.h"

namespace vantagefx {
    namespace viewmodel {

        using std::placeholders::_1;
        using std::placeholders::_2;

        MainViewModel::MainViewModel(VantageFxService &service)
                : _mode(""),
			      _description("Loading stuff..."),
				  _loggedIn(false),
			      _service(service),
			      _refreshTimeout(-1),
			      _lastHour(-1),
                  _lastDay(-1)
        {
			_settings.load();
			setLogin(_settings.login());
			setPassword(_settings.password());
			setServer(_settings.server());

			qRegisterMetaType<LoadingContextPtr>("LoadingContextPtr");
			qRegisterMetaType<RefreshContextPtr>("RefreshContextPtr");
			qRegisterMetaType<AuthContextPtr>("AuthContextPtr");
			qRegisterMetaType<PurchaseContextPtr>("PurchaseContextPtr");
			qRegisterMetaType<std::exception>("std::exception");

			connect(this, &MainViewModel::loadedSignal, this, &MainViewModel::loaded);
			connect(this, &MainViewModel::loadingErrorSignal, this, &MainViewModel::loadingError);
			connect(this, &MainViewModel::refreshedSignal, this, &MainViewModel::refreshed);
			connect(this, &MainViewModel::refreshErrorSignal, this, &MainViewModel::refreshError);
			connect(this, &MainViewModel::authenticatedSignal, this, &MainViewModel::authenticated);
			connect(this, &MainViewModel::authErrorSignal, this, &MainViewModel::authError);
			connect(this, &MainViewModel::purchasedSignal, this, &MainViewModel::purchased);
			connect(this, &MainViewModel::purchaseErrorSignal, this, &MainViewModel::purchaseError);

			doLoad();

			auto timer = new QTimer(this);
			connect(timer, &QTimer::timeout, this, &MainViewModel::update);
			timer->start(100);
		}

		MainViewModel::~MainViewModel() {}

		void MainViewModel::doLoad()
        {
			_service.load(std::bind(&MainViewModel::loadedSignal, this, _1),
				          std::bind(&MainViewModel::loadingErrorSignal, this, _1));
		}

		void MainViewModel::loaded(LoadingContextPtr ctx)
        {
            _model.setLut(ctx->lut());
			_servers.setComboList(_model.servers());
			_assets.setList(_model.assets());
            for(auto assetId : _settings.assets()) {
                _options.watchForAsset(_model.assets()[assetId]);
            }
            emit assetsChanged();
			doRefresh();
        }

        void MainViewModel::loadingError(std::exception e)
        {
			qDebug() << e.what();
			doLoad();
        }

		void MainViewModel::doRefresh()
        {
			_refreshTimeout = -1;
			_service.refresh(_model.instrumentTypeId(),
				             std::bind(&MainViewModel::refreshedSignal, this, _1),
				             std::bind(&MainViewModel::refreshErrorSignal, this, _1));
		}
		
		void MainViewModel::refreshed(RefreshContextPtr ctx)
        {
            if (mode().isEmpty()) setMode("view");
			auto closed = _model.updateOptions(ctx->refresh());
			setMoney(_model.currentMoney());
			for(auto transaction: closed) {
				auto threshold = _options.threshold(transaction.asset().id());
				_stat.update(transaction.asset().name(), transaction.optionIndex(), transaction.bet(), transaction.returned(), threshold);
			}

            bool changed = false;
            for(auto item : _model.options()) {
                if (_settings.assets().contains(item.asset().id())) continue;
                changed = true;
                _settings.appendAsset(item.asset().id());
            }
            if (changed) {
                _settings.save();
            }

			_options.updateAssets(_model.assets());
			_options.updateOptions(_model.options());
            auto bets = _options.calculateVirtualBets();
            _stat.writeVirtualBets(bets);

			if (!makePurchases(_model.options())) {
				_refreshTimeout = 0;
			}
		}

        void MainViewModel::refreshError(std::exception e)
        {
            qDebug() << e.what();
			doRefresh();
			_refreshTimeout = 0;
		}

		void MainViewModel::processLogin()
		{
			setMode("");
			setDescription("Logging in...");
			doAuth(_login, _password, _server);
		}

		void MainViewModel::doAuth(QString login, QString password, QString server)
        {
			_service.auth(login.toStdString(), password.toStdString(), server.toStdString(),
				          std::bind(&MainViewModel::authenticatedSignal, this, _1),
				          std::bind(&MainViewModel::authErrorSignal, this, _1));
		}

		void MainViewModel::authenticated(AuthContextPtr ctx)
		{
			_refreshTimeout = 40;
			_model.setAccount(ctx->auth());
			setLoggedIn(_model.isLoggedIn());
			setFullName(_model.userName());

            _stat.setLogin(login());
			_settings.setLogin(login());
			_settings.setPassword(password());
			_settings.setServer(server());
			_settings.save();
		}

		void MainViewModel::authError(std::exception e)
        {
			qDebug() << e.what();
			setMode("view");
			_refreshTimeout = 40;
		}

	    void MainViewModel::doPurchase(int64_t optionId, int64_t money, int positionType)
        {
			auto accountId = _model.accountId();
			auto option = _model.optionInfo(optionId);
	        auto assetId = option.asset().id();
			auto currentPrice = option.asset().price();
	        _service.buy(accountId, optionId, assetId, money, currentPrice, positionType,
				         std::bind(&MainViewModel::purchasedSignal, this, _1),
				         std::bind(&MainViewModel::purchaseErrorSignal, this, _1));
        }

		void MainViewModel::purchased(PurchaseContextPtr ctx)
        {
			auto result = _model.updatePurchase(ctx->transaction());
			_options.updateOption(std::get<0>(result));
			_refreshTimeout = 20;
		}

		void MainViewModel::purchaseError(std::exception e)
        {
			qDebug() << e.what();
			setMode("view");
			_refreshTimeout = 20;
		}

        void MainViewModel::update() {
			if (_refreshTimeout >= 0 && ++_refreshTimeout == 50) {
				doRefresh();
			}
	        auto hour = QDateTime::currentDateTime().time().hour();
            auto day = QDateTime::currentDateTime().date().day();

			if (_lastHour >= 0 && hour != _lastHour) {
				_stat.writeHourLine();
			}
            if (_lastDay >= 0 && day != _lastDay) {
                _stat.writeDayLine();
            }
			_lastHour = hour;
            _lastDay = day;
        }

		void MainViewModel::doLogin()
		{
            setMode("login");
		}

        void MainViewModel::cancelLogin()
        {
            setMode("view");
        }

        void MainViewModel::view(int64_t optionId)
        {
            setMode("details");
            auto info = _model.optionInfo(optionId);
            setCurrentOption(info);
            setOptionName(info.asset().name());
            if (info.seconds() >= 120) {
                setOptionExpire(QString::number(info.seconds() / 60) + "m");
            }
            else {
                setOptionExpire(QString::number(info.seconds()) + "s");
            }
        }

	    void MainViewModel::setBet(int firstBet, int betGrowth)
        {
			_model.setFirstBet(firstBet);
			_model.setBetGrowth(betGrowth);
            _options.updateOptions(_model.options());
			emit firstBetChanged();
			emit betGrowthChanged();
        }

        void MainViewModel::watch(int index)
        {
            auto assetId = _assets.assetId(index);
            auto asset = _model.assetInfo(assetId);
            _options.watchForAsset(asset);
			_settings.appendAsset(assetId);
			_settings.save();
        }

        void MainViewModel::stopWatch(int assetId)
        {
            _options.stopWatch(assetId);
            _settings.removeAsset(assetId);
            _settings.save();
        }

	    void MainViewModel::setMode(const QString &mode)
        {
            if (_mode == mode) return;
            _mode = mode;
            emit modeChanged();
        }

		void MainViewModel::setDescription(const QString &description)
        {
			if (_description == description) return;
			_description = description;
			emit descriptionChanged();
        }

		void MainViewModel::setLogin(const QString &login)
		{
            if (_login == login) return;
            _login = login;
            emit loginChanged();

		}

		void MainViewModel::setPassword(const QString &password)
		{
            if (_password == password) return;
            _password = password;
            emit passwordChanged();
		}

		void MainViewModel::setServer(const QString &server)
		{
            if (_server == server) return;
            _server = server;
            emit serverChanged();

		}

		void MainViewModel::setLoggedIn(bool loggedIn)
        {
			if (_loggedIn == loggedIn) return;
			_loggedIn = loggedIn;
			emit loggedInChanged();
        }

        void MainViewModel::setFullName(const QString &fullName) 
		{
            if (_fullName == fullName) return;
            _fullName = fullName;
            emit fullNameChanged();
        }

        void MainViewModel::setMoney(int64_t money) {
			_money = QString("$ %L1").arg(static_cast<double>(money / 100), 0, 'f', 2);
            emit moneyChanged();
        }

        void MainViewModel::setCurrentOption(const model::OptionModel &option)
        {
            _currentOption = option;
        }

        void MainViewModel::setOptionName(const QString &optionName) {
            if (optionName == _optionName) return;
            _optionName = optionName;
            emit optionNameChanged();
        }

        void MainViewModel::setOptionReturn(int optionReturn) {
            if (optionReturn == _optionReturn) return;
            _optionReturn = optionReturn;
            emit optionReturnChanged();
        }

        void MainViewModel::setOptionExpire(const QString &optionExpire) {
            if (optionExpire == _optionExpire) return;
            _optionExpire = optionExpire;
            emit optionExpireChanged();
        }

	    bool MainViewModel::makePurchases(const QMap<int64_t, model::OptionModel> &options)
        {
			if (options.empty()) return false;
			if (!_model.isLoggedIn()) return false;
			for (auto& option: options) {
				if (!option.isAvailable()) continue;
				if (!_options.isSelected(option.optionId())) continue;
				if (_model.hasTransactionFor(option.optionId())) {
					qDebug("second buy");
					continue;
				}
				auto threshold = _options.threshold(option.asset().id());
				if (std::abs(threshold) < 50) threshold = 71;

				if (option.highRateValue() >= std::abs(threshold)) {
					doPurchase(option.optionId(), option.currentBet() * 100, threshold > 0 ? _model.rateId("Put") : _model.rateId("Call"));
					return true;
				}
				if (option.lowRateValue() >= std::abs(threshold)) {
					doPurchase(option.optionId(), option.currentBet() * 100, threshold > 0 ? _model.rateId("Call") : _model.rateId("Put"));
					return true;
				}
			}
			return false;
		}

	    const QString &MainViewModel::money() const
        {
            return _money;
        }
	}
}
