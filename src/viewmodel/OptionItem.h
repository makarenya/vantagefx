//
// Created by AMakarenya on 07.06.2016.
//

#ifndef VANTAGEFX_OPTIONITEM_H
#define VANTAGEFX_OPTIONITEM_H

#include <QtCore>
#include "../model/OptionModel.h"


namespace vantagefx {
    namespace viewmodel {

        struct OptionItem {

            OptionItem(int s = 0);

            int64_t id;
            model::OptionModel::OptionStatus status;
            int bet;
            int seconds;
            bool selected;

            bool toggle();
            QString name() const;
            QColor background() const;
            QColor border() const;
            QColor foreground() const;
        };
    }
}


#endif //VANTAGEFX_OPTIONITEM_H
