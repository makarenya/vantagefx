//
// Created by alexx on 27.04.2016.
//

#include "OptionsListModel.h"
#include <QColor>
#include <sstream>
#include <iomanip>


namespace vantagefx {
    namespace viewmodel {

		OptionsListModel::OptionsListModel(QObject *parent)
			: QAbstractListModel(parent),
			  _assetId(0),
			  _rateHi(0),
			  _rateLow(0),
			  _price(0.0),
			  _threshold(71),
			  _options({ OptionItem(30), OptionItem(60), OptionItem(120), OptionItem(300) })
        {
            _sequence.push_back(TimePoint(QDateTime::currentDateTime(), 0));
            for(int i = 0; i < _options.size(); ++i) {
                _options[i].timePoint = _sequence.begin();
            }
        }

	    int OptionsListModel::rowCount(const QModelIndex &parent) const
        {
			return _options.size();
        }

	    QVariant OptionsListModel::data(const QModelIndex &index, int role) const
        {
			if (!index.isValid()) return QVariant();
			if (index.row() >= _options.size() || index.column() != 0) return QVariant();
			switch(role) {
                case IdRole: return _options[index.row()].id;
                case StatusRole: return _options[index.row()].status;
                case NameRole: return _options[index.row()].name();
                case BackgroundRole: return _options[index.row()].background();
                case BorderRole: return _options[index.row()].border();
                case ForegroundRole: return _options[index.row()].foreground();
                case BetRole: return _options[index.row()].bet;
                case SelectedRole: return _options[index.row()].selected;
                default: return QVariant();
			}
        }

        bool OptionsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
        {
            if (!index.isValid()) return false;
            if (index.row() >= _options.size()) return false;
            if (role != SelectedRole) return false;
            auto &option = _options[index.row()];
            if (option.selected == value.toBool()) return true;
            option.selected = value.toBool();
            dataChanged(index, index, { SelectedRole, BackgroundRole, ForegroundRole, BorderRole });
            return true;
        }

	    QHash<int, QByteArray> OptionsListModel::roleNames() const
		{
			QHash<int, QByteArray> result;
			result[IdRole] = "id";
			result[NameRole] = "name";
			result[StatusRole] = "status";
			result[BackgroundRole] = "background";
			result[BorderRole] = "border";
			result[ForegroundRole] = "foreground";
			result[BetRole] = "bet";
            result[SelectedRole] = "selected";
			return result;
		}

	    void OptionsListModel::updateOption(const model::OptionModel &item)
        {
	        auto i = item.index();
			auto &result = _options[i];
			QVector<int> roles;
			if (result.id != item.optionId()) {
				result.id = item.optionId();
				roles.push_back(IdRole);
			}
			if (result.seconds != item.seconds()) {
				result.seconds = item.seconds();
				roles.push_back(NameRole);
			}
			if (result.status != item.status()) {
				result.status = item.status();
				roles.push_back(StatusRole);
				roles.push_back(BackgroundRole);
				roles.push_back(BorderRole);
				roles.push_back(ForegroundRole);
			}
			if (result.bet != item.currentBet()) {
				result.bet = item.currentBet();
				roles.push_back(BetRole);
			}
			dataChanged(index(i), index(i), roles);
        }

	    void OptionsListModel::ids(QSet<int64_t> &list)
        {
			for (auto &item : _options) {
				if (item.id != 0) list.insert(item.id);
			}
        }

	    bool OptionsListModel::remove(QSet<int64_t>& list)
        {
	        auto empty = 0;
			for (auto j = 0; j < _options.size(); ++j) {
				auto &option = _options[j];
				if (list.contains(_options[j].id)) {
					option.id = 0;
					option.status = model::OptionModel::NotFound;
					option.bet = 0;
					dataChanged(index(j), index(j), { IdRole, StatusRole, BackgroundRole, BorderRole, ForegroundRole, BetRole });
				}
				if (_options[j].id == 0) empty++;
			}
			return empty == _options.size();
        }

		bool OptionsListModel::contains(int64_t optionId) const
		{
			for (auto &item : _options) {
				if (item.id == optionId) return true;
			}
			return false;
		}

		bool OptionsListModel::isSelected(int64_t optionId) const
		{
			for (auto &item : _options) {
				if (item.id == optionId) return item.selected;
			}
			throw std::out_of_range("option " + std::to_string(optionId) + " not found");
		}

        QList<VirtualBet> OptionsListModel::calculateVirtualBets()
        {
            if (_rateHi == 0 || _rateLow == 0) return QList<VirtualBet>();
            // Текущая засечка по времени
            auto now = TimePoint(QDateTime::currentDateTime(), _price, _rates);
            // Если до сих пор в последовательности засечек ничего нет,
            // то опции не содержат правильный указатель на свою вершину.
            if (_sequence.empty()) {
                _sequence.push_back(now);
                // Инициализируем указатели для опций
                for(int i = 0; i < _options.size(); ++i) {
                    _options[i].initializeTimePoint(_sequence.begin());
                }
            }
            else {
                // Либо просто добавляем засечку в последовательность
                _sequence.push_back(now);
            }
            // Список результатов
            QList<VirtualBet> bets;

            // Добавляем результаты
            for(int i = 0; i < _options.size(); ++i) {
				auto bet = _options[i].calculateVirtualBet(now);
				if (!bet.empty()) {
					auto pnt = _options[i].timePoint;
					int dynLevel = 0;
					for (auto it = _sequence.begin(); it != _sequence.end(); ++it) {
						int diff = it->time().secsTo(pnt->time());
						
						int dynamics = it->price() == 0 ? 0 : round((pnt->price() - it->price()) * 100000 / it->price());
						if (dynLevel == 0 && diff < 120) {
							bet.setLongDynamic(dynamics);
							dynLevel = 1;
						}
						if (dynLevel == 1 && diff < 60) {
							bet.setMidDynamic(dynamics);
							dynLevel = 2;
						}
						if (dynLevel == 2 && diff < 30) {
							bet.setShortDynamic(dynamics);
							break;
						}
					}
				}
                bets.append(bet);
            }
            // Если последовательность перерасла 6-минутный рубеж, обрезаем её.
            if (_sequence.first().time().secsTo(now.time()) > 600) {
                _sequence.removeFirst();
            }
            // Массив результатов
            return bets;
        }

        void OptionsListModel::setMarketId(int marketId)
        {
            _marketId = marketId;
        }

	    void OptionsListModel::setAssetId(int assetId)
        {
	        _assetId = assetId;
        }

	    void OptionsListModel::setName(QString name)
        {
	        _name = name;
        }

	    void OptionsListModel::setRateHi(int rateHi)
        {
	        _rateHi = rateHi;
        }

		void OptionsListModel::setAllRates(const QMap<QString, int> &rates)
		{
			_rates = rates;
		}

	    void OptionsListModel::setRateLow(int rateLow)
        {
	        _rateLow = rateLow;
        }

	    void OptionsListModel::setPrice(double price)
        {
	        _price = price;
        }

	    int64_t OptionsListModel::order() const
        {
            return (marketId() << 20) | assetId();
        }

	    void OptionsListModel::setThreshold(int threshold)
        {
	        _threshold = threshold;
        }
	}
}