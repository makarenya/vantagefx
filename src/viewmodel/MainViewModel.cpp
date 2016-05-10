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
            if (_controller.isError()) {
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
                    emit serversChanged(&_servers);
					setLoggedIn(_controller.isLoggedIn());
                    setFullName(_controller.fullName().c_str());
                    _options.updateOptions(std::move(_controller.options()));
                    _controller.finish();
                }
			}
            if (_mode == "view") {
                if (_controller.isReady()) {
                    _options.updateOptions(std::move(_controller.options()));
                    setMoney(_controller.money());
                    _controller.finish();
					makePurchases();
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

        void MainViewModel::view(long long optionId)
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
            emit modeChanged(mode);
        }

		void MainViewModel::setDescription(const QString &description)
        {
			if (_description == description) return;
			_description = description;
			emit descriptionChanged(description);
        }

		void MainViewModel::setLogin(const QString &login)
		{
            if (_login == login) return;
            _login = login;
            emit loginChanged(_login);

		}

		void MainViewModel::setPassword(const QString &password)
		{
            if (_password == password) return;
            _password = password;
            emit passwordChanged(_password);
		}

		void MainViewModel::setServer(const QString &server)
		{
            if (_server == server) return;
            _server = server;
            emit serverChanged(_server);

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
            emit fullNameChanged(_fullName);
        }

        void MainViewModel::setMoney(int64_t money) {
            QString sub = QString::number(money % 100);
            money /= 100;
            QString ones = QString::number(money % 1000);
            money /= 1000;
            auto value = "$ " + ((money == 0) ? ones + "." + sub :
                         QString::number(money) + " " + ones + "." + sub);
            if (value == _money) return;
            _money = value;
            emit moneyChanged(_money);
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

	    void MainViewModel::makePurchases()
        {
        }

	    const QString &MainViewModel::money() const
        {
            return _money;
        }
    }
}
