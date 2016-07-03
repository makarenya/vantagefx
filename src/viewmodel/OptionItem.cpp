//
// Created by AMakarenya on 07.06.2016.
//

#include "OptionItem.h"
#include <QColor>


namespace vantagefx {
    namespace viewmodel {

        OptionItem::OptionItem(int s)
                : id(0),
                  status(model::OptionModel::NotFound),
                  bet(0),
				  seconds(s),
				  selected(true)
        {}

		bool OptionItem::toggle()
		{
			return selected = !selected;
		}

		QString OptionItem::name() const
		{
			if (seconds >= 120) {
				return QString("%1m").arg(seconds / 60);
			}
			return QString("%1").arg(seconds);
		}

        QColor OptionItem::background() const
        {
            switch (status) {
                case model::OptionModel::Idle:
					if (selected) return QColor("#e4f0fa");
					return QColor(Qt::white);
                case model::OptionModel::Processing:
					return QColor("#565656");
                case model::OptionModel::Successful:
					return QColor("#5bff5b");
                case model::OptionModel::Failed:
					return QColor("#ff2828");
                case model::OptionModel::Returned:
					return QColor("#f7ff28");
                default:
                    if (selected) return QColor("#e7ecf1");
                    return QColor("#f3f3f3");
            }
        }

		QColor OptionItem::border() const
		{
			switch (status) {
                case model::OptionModel::Idle:
                    if (selected) return QColor("#0076d4");
                    return QColor("#b4b4b5");
                case model::OptionModel::Processing:
                    return QColor(Qt::black);
                case model::OptionModel::Successful:
                    return QColor("#009f00");
                case model::OptionModel::Failed:
                    return QColor("#810000");
                case model::OptionModel::Returned:
    				return QColor("#b6bd00");
                default:
                    if (selected) return QColor("#a3c1d9");
                    return QColor("#e1e1e1");
			}
		}

		QColor OptionItem::foreground() const
		{
			switch (status) {
                case model::OptionModel::Idle:
                    if (selected) return QColor(Qt::black);
                    return QColor(Qt::black);
                case model::OptionModel::Processing:
                    return QColor(Qt::white);
                case model::OptionModel::Successful:
                    return QColor(Qt::black);
                case model::OptionModel::Failed:
                    return QColor(Qt::white);
                case model::OptionModel::Returned:
                    return QColor(Qt::black);
                default:
                    if (selected) return QColor("#e4f0fa");
                    return QColor(Qt::white);
			}
		}

        VirtualBet OptionItem::calculateVirtualBet(const TimePoint &start, const TimePoint &stop)
        {
            auto bet = VirtualBet(seconds);
			if (status == model::OptionModel::NotFound) return bet;
			if (start.time().secsTo(stop.time()) <= seconds) return bet;

			double diff = stop.price() - start.price();

            if (diff > 0) {
                bet.setHigh();
            }
            else if (diff < 0) {
                bet.setLow();
            }
            else {
                bet.setParity();
            }
			bet.setTime(start.time());
            bet.setRates(start.rates());
            return bet;
        }
    }
}