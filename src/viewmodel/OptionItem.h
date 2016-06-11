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

            TimePoint(QDateTime time = QDateTime(), double price = 0, QMap<QString, int> rates = QMap<QString, int>())
                    : _time(time), _price(price), _rates(rates) { }

            QDateTime time() const { return _time; }
            double price() const { return _price; }
            const QMap<QString, int> &rates() const { return _rates; };

        private:
            QDateTime _time;
            double _price;
            QMap<QString, int> _rates;
        };

        class VirtualBet
        {
        public:

            VirtualBet(int seconds)
                    : _seconds(seconds), _result(0), _empty(true) {}

            void setTime(const QDateTime &time) { _time = time; }
            QDateTime time() const { return _time; }
            int seconds() const { return _seconds; }
            int result() const { return _result; }
            int rate(QString name) const { return _rates[name]; }
            void setRates(const QMap<QString, int> &rates) { _rates = rates; }
            bool empty() const { return _empty; }

            void setHigh() { _result = 1; _empty = false; }
            void setLow() { _result = -1; _empty = false; }
            void setParity() { _result = 0; _empty = false; }

        private:
            QDateTime _time;
            int _seconds;
            int _result;
            QMap<QString, int> _rates;
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
