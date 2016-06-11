//
// Created by AMakarenya on 07.06.2016.
//

#ifndef VANTAGEFX_OPTIONITEM_H
#define VANTAGEFX_OPTIONITEM_H

#include <QtCore>
#include "../model/OptionModel.h"


namespace vantagefx {
    namespace viewmodel {

        class TimePoint
        {
        public:

            TimePoint(QDateTime time = QDateTime(), double price = 0, int rate = 0)
                    : _time(time), _price(price), _rate(rate) { }

            QDateTime time() const { return _time; }
            double price() const { return _price; }
            int rate() const { return _rate; };

        private:
            QDateTime _time;
            double _price;
            int _rate;
        };

        class VirtualBet
        {
        public:

            VirtualBet(QDateTime time)
                    : _time(time), _result(0), _rate(0), _empty(true) {}

            QDateTime time() const { return _time; }
            int result() const { return _result; }
            int rate() const { return _rate; }
            void setRate(int rate) { _rate = rate; }
            bool empty() const { return _empty; }

            void setHigh() { _result = 1; _empty = false; }
            void setLow() { _result = -1; _empty = false; }
            void setParity() { _result = 0; _empty = false; }

        private:
            QDateTime _time;
            int _result;
            int _rate;
            bool _empty;
        };

        struct OptionItem
        {
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

            void initializeTimePoint(QLinkedList<TimePoint>::iterator timePoint);
            VirtualBet calculateVirtualBet(TimePoint &now);

            QLinkedList<TimePoint>::iterator timePoint;
        };
    }
}


#endif //VANTAGEFX_OPTIONITEM_H
