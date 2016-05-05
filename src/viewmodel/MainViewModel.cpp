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
				QThread::sleep(10);
			}
		}

        void MainViewModel::cancelLogin()
        {
            setMode("view");
        }

        void MainViewModel::view(long long optionId)
        {
            setMode("details");
            auto info = _controller.optionInfo(optionId);
            setOptionName(info.name.c_str());
            setOptionReturn(info.returnValue);
            setOptionExpire(info.expires.c_str());
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

        void MainViewModel::setFullName(const QString &fullName) {
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

        const QString &MainViewModel::money() const
        {
            return _money;
        }
    }
}
