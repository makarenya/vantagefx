//
// Created by alexx on 27.04.2016.
//

#include "OptionsListModel.h"
#include <sstream>
#include <iomanip>


namespace vantagefx {
    namespace viewmodel {

        OptionsListModel::OptionsListModel(QObject *parent)
                : QAbstractListModel(parent)
        {
        }
		
		QHash<int, QByteArray> OptionsListModel::roleNames() const
        {
            QHash<int, QByteArray> roles;
            roles[Qt::DisplayRole] = "display";
            roles[AssetIdRole] = "asset";
            roles[NameRole] = "name";
            roles[MoneyBackRole] = "back";
            roles[RateLowRole] = "low";
            roles[RateHiRole] = "high";
            roles[PriceRole] = "price";
            roles[Option30Role] = "option_30";
            roles[Option60Role] = "option_60";
            roles[Option120Role] = "option_120";
            roles[Option300Role] = "option_300";
            roles[ThresholdRole] = "threshold";
			roles[Color30Role] = "color_30";
			roles[Color60Role] = "color_60";
			roles[Color120Role] = "color_120";
			roles[Color300Role] = "color_300";
			return roles;
        }

        QVariant OptionsListModel::data(const QModelIndex &index, int role) const
        {
            if (!index.isValid()) return QVariant();
            if (index.row() > _items.size()) return QVariant();

            auto option = _items[index.row()];

            switch(role) {
                case Qt::DisplayRole:
                case NameRole:
                    return option.name;
                case AssetIdRole:
                    return option.assetId;
                case MoneyBackRole:
                    return option.moneyBack;
                case RateLowRole:
                    return option.rateLow;
                case RateHiRole:
                    return option.rateHi;
                case PriceRole:
                    return option.price;
                case Option30Role:
                    return option.option30;
                case Option60Role:
                    return option.option60;
                case Option120Role:
                    return option.option120;
                case Option300Role:
                    return option.option300;
                case ThresholdRole:
                    return option.threshold;
				case Color30Role:
					return option.color30;
				case Color60Role:
					return option.color60;
				case Color120Role:
					return option.color120;
				case Color300Role:
					return option.color300;
				default:
                    return QVariant();
            }
        }

        bool OptionsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
        {
            if (!index.isValid()) return false;
            if (index.row() > _items.size()) return false;
            if (role != ThresholdRole) return false;
            _items[index.row()].threshold = value.toInt();
            return true;
        }

		void OptionsListModel::select(long long optionId)
		{
			
			qDebug() << "selected " << optionId;
		}

        int OptionsListModel::rowCount(const QModelIndex &parent) const
        {
            auto cnt = _items.size();
			return cnt;
        }

		QVector<int> OptionsListModel::updateOption(OptionListItem &current, model::OptionModel &item)
        {
			QVector<int> roles;
			if (item.asset().name() != current.name) {
				current.name = item.asset().name();
				roles.push_back(NameRole);
			}
			if (item.moneyBack() != current.moneyBack) {
				current.moneyBack = item.moneyBack();
				roles.push_back(MoneyBackRole);
			}
			if (item.lowRateValue() != current.rateLow) {
				current.rateLow = item.lowRateValue();
				roles.push_back(RateLowRole);
			}
			if (item.highRateValue() != current.rateHi) {
				current.rateHi = item.highRateValue();
				roles.push_back(RateHiRole);
			}
			if (item.price() != current.price) {
				current.price = item.price();
				roles.push_back(PriceRole);
			}
			if (item.seconds() == 30) {
				if (item.optionId() != -current.option30) roles.push_back(Option30Role);
				current.option30 = item.optionId();
				if (color(item) != current.color30) roles.push_back(Color30Role);
				current.color30 = color(item);
			}
			if (item.seconds() == 60) {
				if (item.optionId() != -current.option60) roles.push_back(Option60Role);
				current.option60 = item.optionId();
				if (color(item) != current.color60) roles.push_back(Color60Role);
				current.color60 = color(item);
			}
			if (item.seconds() == 120) {
				if (item.optionId() != -current.option120) roles.push_back(Option120Role);
				current.option120 = item.optionId();
				if (color(item) != current.color120) roles.push_back(Color120Role);
				current.color120 = color(item);
			}
			if (item.seconds() == 300) {
				if (item.optionId() != -current.option300) roles.push_back(Option300Role);
				current.option300 = item.optionId();
				if (color(item) != current.color300) roles.push_back(Color300Role);
				current.color300 = color(item);
			}
			return roles;
        }

		OptionListItem OptionsListModel::createOption(model::OptionModel &item, std::string lineId)
        {
			OptionListItem inserted;
			inserted.lineId = lineId.c_str();
			inserted.assetId = item.asset().id();
			inserted.name = item.asset().name();
			inserted.moneyBack = item.moneyBack();
			inserted.rateLow = item.lowRateValue();
			inserted.rateHi = item.highRateValue();
			inserted.price = item.price();
			if (item.seconds() == 30) {
				inserted.option30 = item.optionId();
				inserted.color30 = color(item);
			}
			if (item.seconds() == 60) {
				inserted.option60 = item.optionId();
				inserted.color60 = color(item);
			}
			if (item.seconds() == 120) {
				inserted.option120 = item.optionId();
				inserted.color120 = color(item);
			}
			if (item.seconds() == 300) {
				inserted.option300 = item.optionId();
				inserted.color300 = color(item);
			}
			return inserted;
        }

        void OptionsListModel::updateOptions(const QMap<int64_t, model::OptionModel> &options)
        {
			if (options.empty()) return;
			for (auto &current : _items) {
				current.option30 = -current.option30;
				current.option60 = -current.option60;
				current.option120 = -current.option120;
				current.option300 = -current.option300;
			}
			for(auto item: options) {
	            auto added = false;
				std::stringstream linestream;
				linestream << std::setw(4) << std::setfill('0') << item.asset().marketId() << "." 
					<< std::setw(6) << std::setfill('0') << item.asset().id();

				auto lineId = linestream.str();

                for (auto i = 0; i < _items.size(); ++i) {
                    auto &current = _items[i];
                    if (item.asset().id() == current.assetId) {
						added = true;
						auto roles = updateOption(current, item);
						if (roles.size() > 0) {
							dataChanged(index(i, 0), index(i, 0), roles);
						}
						break;
                    }
                    else if (lineId.c_str() < current.lineId) {
						added = true;
						auto inserted = createOption(item, lineId);
                        beginInsertRows(QModelIndex(), i, i);
                        _items.insert(i, inserted);
                        endInsertRows();
						break;
                    }
                }
                if (!added)
				{
					auto inserted = createOption(item, lineId);
					beginInsertRows(QModelIndex(), _items.size(), _items.size());
					_items.push_back(inserted);
					endInsertRows();
				}
			}
            for (auto i = 0; i < _items.size(); ++i) {
                auto &current = _items[i];
                QVector<int> roles;
                if (current.option30 < 0) {
                    current.option30 = 0;
                    roles.push_back(Option30Role);
                }
                if (current.option60 < 0) {
                    current.option60 = 0;
                    roles.push_back(Option60Role);
                }
                if (current.option120 < 0) {
                    current.option120 = 0;
                    roles.push_back(Option120Role);
                }
                if (current.option300 < 0) {
                    current.option300 = 0;
                    roles.push_back(Option300Role);
                }
				if (roles.size() == 4) {
					beginRemoveRows(QModelIndex(), i, i);
					_items.removeAt(i--);
					endRemoveRows();
				}
                else if (roles.size() > 0) {
                    dataChanged(index(i, 0), index(i, 0), roles);
                }
            }
        }

        int OptionsListModel::threshold(int assetId) {
            for(auto item : _items) {
                if (item.assetId == assetId) return item.threshold;
            }
            return 0;
        }

	    QString OptionsListModel::color(model::OptionModel& option)
        {
	        switch(option.status()) {
			case model::OptionModel::Idle:
				return "#FFFFFF";
		    case model::OptionModel::Selected:
				return "#1882d7";
		    case model::OptionModel::Processing:
				return "#000000";
			case model::OptionModel::Successful:
				return "#00FF00";
			case model::OptionModel::Failed:
				return "#FF0000";
			default:
				return "#FFFF00";
			}
        }
    }
}