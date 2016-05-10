//
// Created by alexx on 09.03.2016.
//
#include "MainViewModel.h"
#include <QMessageBox>

namespace vantagefx {
    namespace viewmodel {

        MainViewModel::MainViewModel(Controller &controller)
                : _mode(""),
                  _login("123301954"),
                  _password("eNJ0D"),
				  _loggedIn(false),
                  _controller(controller),
                  _refreshTimeout(0)
        {
			_controller.load();
            setDescription("Loading stuff...");
		
			auto timer = new QTimer(this);
			connect(timer, &QTimer::timeout, this, &MainViewModel::update);
			timer->start(100);
		}

        MainViewModel::~MainViewModel() {
        }

        void MainViewModel::update() {
			if (_mode == "purchasing") {
				if (_controller.isReady()) {
					_assetLimits[currentOption().assetId()] = std::chrono::steady_clock::now() + std::chrono::seconds(70);
					_controller.finish();
					_refreshTimeout = 49;
					setMode("view");
				}
				if (_controller.isError()) {
					qDebug(_controller.exception().what());
					setMode("view");
					_controller.finish();
					_refreshTimeout = 49;
				}
			}
			else if (_controller.isError()) {
				qDebug(_controller.exception().what());
				setMode("");
                _controller.finish();
                _controller.load();
            }
			if (_mode.isEmpty()) {
				if (_controller.isReady()) {
                    setMode("view");
                    QStringList servers;
                    for(auto server : _controller.servers()) {
                        servers.push_back(server.c_str());
                    }
                    _servers.setComboList(servers);
                    emit serversChanged();
					setLoggedIn(_controller.isLoggedIn());
                    setFullName(_controller.fullName().c_str());
					auto options = std::move(_controller.options());
					_controller.finish();
					makePurchases(options);
                }
			}
            if (_mode == "view") {
                if (_controller.isReady()) {
					setMoney(_controller.money());
					auto options = std::move(_controller.options());
					_controller.finish();
					makePurchases(options);
					_refreshTimeout = 0;
                }
                if (_refreshTimeout >= 0 && ++_refreshTimeout == 50) {
                    _refreshTimeout = -1;
                    _controller.refresh();
                }
            }
        }

		void MainViewModel::doLogin()
		{
            setMode("login");
		}

		void MainViewModel::processLogin()
		{
			setMode("");
            setDescription("Logging in...");
			while (!_controller.auth(_login.toStdString(), _password.toStdString(), _server.toStdString())) {
				QCoreApplication::processEvents();
			}
		}

        void MainViewModel::cancelLogin()
        {
            setMode("view");
        }

        void MainViewModel::view(int64_t optionId)
        {
			_controller.wait();
            setMode("details");
            auto info = _controller.optionInfo(optionId);
            setCurrentOption(info);
            setOptionName(info.name().c_str());
            setOptionReturn(info.returnValue());
            if (info.seconds() >= 120) {
                setOptionExpire(QString::number(info.seconds() / 60) + "m");
            }
            else {
                setOptionExpire(QString::number(info.seconds()) + "s");
            }
        }

        void MainViewModel::buyHigh()
        {
			setMode("");
			setDescription("Process purchase...");
			_controller.buy(currentOption().optionId(), 10000, _controller.rateId("Put"));
		}

        void MainViewModel::buyLow()
        {
			setMode("");
			setDescription("Process purchase...");
			_controller.buy(currentOption().optionId(), 10000, _controller.rateId("Call"));
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
	        auto sub = QString::number(money % 100);
            money /= 100;
	        auto ones = QString::number(money % 1000);
            money /= 1000;
            auto value = "$ " + ((money == 0) ? ones + "." + sub :
                         QString::number(money) + " " + ones + "." + sub);
            if (value == _money) return;
            _money = value;
            emit moneyChanged();
        }

        void MainViewModel::setCurrentOption(const model::GwtOptionModel &option)
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

	    void MainViewModel::makePurchases(std::map<int64_t, model::GwtOptionModel> &options)
        {
			_options.updateOptions(options);
			if (!_controller.isLoggedIn()) return;
			for(auto opt: options) {
				auto option = opt.second;
				if (option.seconds() != 120) continue;
				auto assetLimit = _assetLimits.find(option.assetId());
				if (assetLimit != _assetLimits.end() && assetLimit->second > std::chrono::steady_clock::now()) continue;
				if (option.rate("Put") > 70) {
					setMode("purchasing");
					setCurrentOption(option);
					setDescription("Processing " + QString::fromStdString(option.name()) + "...");
					_controller.buy(option.optionId(), 10000, _controller.rateId("Put"));
					_refreshTimeout = -1;
					return;
				}
				if (option.rate("Call") > 70) {
					setMode("purchasing");
					setCurrentOption(option);
					setDescription("Processing " + QString::fromStdString(option.name()) + "...");
					_controller.buy(option.optionId(), 10000, _controller.rateId("Call"));
					_refreshTimeout = -1;
					return;
				}
			}
		}

	    const QString &MainViewModel::money() const
        {
            return _money;
        }
    }
}
