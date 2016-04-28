//
// Created by alexx on 09.03.2016.
//

#ifndef QT_SECOND_MAINVIEWMODEL_H
#define QT_SECOND_MAINVIEWMODEL_H

#include <QtCore>
#include "../model/MainModel.h"
#include "../viewmodel/OptionsListModel.h"
#include "Controller.h"
#include "PersonViewModel.h"

namespace vantagefx {
    namespace viewmodel {

        class MainViewModel : public QObject
        {
        Q_OBJECT

        public:

            Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged);
            Q_PROPERTY(OptionsListModel *options READ options NOTIFY optionsChanged)

        public:
            explicit MainViewModel(Controller &&controller);

            virtual ~MainViewModel();

            bool loaded() const { return _loaded; }

            OptionsListModel *options() { return &_options; }

        public slots:

            void update();

        signals:

            void loadedChanged(bool loaded);

            void optionsChanged(const OptionsListModel *options);

        private:

            OptionsListModel _options;
            bool _loaded;
            Controller _controller;
            int _refreshTimeout;

			MainViewModel(const MainViewModel &rhs);
			MainViewModel &operator=(const MainViewModel &rhs);
        };
    }
}


#endif //QT_SECOND_MAINVIEWMODEL_H
