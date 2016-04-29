//
// Created by alexx on 09.03.2016.
//
#include "MainViewModel.h"
#include <QMessageBox>

namespace vantagefx {
    namespace viewmodel {

        MainViewModel::MainViewModel(Controller &&controller)
                : _loaded(false),
                  _controller(std::move(controller)),
                  _refreshTimeout(-1)
        {
			_controller.load([this](Controller &) {
                _controller.refresh([this](Controller &) {
					_refreshTimeout = 0;
                });
            });
		
			auto timer = new QTimer(this);
			connect(timer, &QTimer::timeout, this, &MainViewModel::update);
			timer->start(1000);
		}

        MainViewModel::~MainViewModel() {
        }

        void MainViewModel::update() {
			if (_refreshTimeout < 0) return;
            if (_refreshTimeout == 0) {
                _options.updateOptions(std::move(_controller.options()));
				if (!_loaded) {
					if (!_controller.isSuccessfull()) {
						QMessageBox msgBox;
						msgBox.setText(_controller.exception().what());
						msgBox.exec();
					}
					_loaded = true;
					emit loadedChanged(_loaded);
				}
			}
            if (++_refreshTimeout == 5) {
                _refreshTimeout = -1;
                _controller.refresh([this](Controller &) {
                    _refreshTimeout = 0;
                });
            }
        }
    }
}
