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

			const QDateTime &time() const { return _time; }
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
                    : _seconds(seconds), _result(0), _empty(true), _shortDynamic(0), _midDynamic(0), _longDynamic(0) {}

            void setTime(const QDateTime &time) { _time = time; }
			QDateTime time() const { return _time; }
            int seconds() const { return _seconds; }
            int result() const { return _result; }
            int rate(QString name) const { return _rates.contains(name) ? _rates[name] : 50; }
            void setRates(const QMap<QString, int> &rates) { _rates = rates; }
			void setShortDynamic(int dynamic) { _shortDynamic = dynamic;  }
			void setMidDynamic(int dynamic) { _midDynamic = dynamic; }
			void setLongDynamic(int dynamic) { _longDynamic = dynamic;  }
			int shortDynamic() const { return _shortDynamic;  }
			int midDynamic() const { return _midDynamic; }
			int longDynamic() const { return _longDynamic; }
			bool empty() const { return _empty || _rates.size() == 0; }

            void setHigh() { _result = 1; _empty = false; }
            void setLow() { _result = -1; _empty = false; }
            void setParity() { _result = 0; _empty = false; }

        private:
			QDateTime _time;
            int _seconds;
            int _result;
            QMap<QString, int> _rates;
            bool _empty;
			int _shortDynamic;
			int _midDynamic;
			int _longDynamic;
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

            VirtualBet calculateVirtualBet(const TimePoint &start, const TimePoint &stop);
        };
    }
}


#endif //VANTAGEFX_OPTIONITEM_H
