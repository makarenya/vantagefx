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
                default:
                    return QVariant();
            }
        }

        int OptionsListModel::rowCount(const QModelIndex &parent) const
        {
            auto cnt = _items.size();
			return cnt;
        }

		QVector<int> OptionsListModel::updateOption(OptionListItem &current, model::GwtOptionModel &item, int hi, int lo)
        {
			QVector<int> roles;
			if (item.name() != current.name.toStdString()) {
				current.name = item.name().c_str();
				roles.push_back(NameRole);
			}
			if (item.moneyBack() != current.moneyBack) {
				current.moneyBack = item.moneyBack();
				roles.push_back(MoneyBackRole);
			}
			if (lo != current.rateLow) {
				current.rateLow = lo;
				roles.push_back(RateLowRole);
			}
			if (hi != current.rateHi) {
				current.rateHi = hi;
				roles.push_back(RateHiRole);
			}
			if (item.price() != current.price) {
				current.price = item.price();
				roles.push_back(PriceRole);
			}
			if (item.seconds() == 30) {
				if (item.optionId() != -current.option30) roles.push_back(Option30Role);
				current.option30 = item.optionId();
			}
			if (item.seconds() == 60) {
				if (item.optionId() != -current.option60) roles.push_back(Option60Role);
				current.option60 = item.optionId();
			}
			if (item.seconds() == 120) {
				if (item.optionId() != -current.option120) roles.push_back(Option120Role);
				current.option120 = item.optionId();
			}
			if (item.seconds() == 300) {
				if (item.optionId() != -current.option300) roles.push_back(Option300Role);
				current.option300 = item.optionId();
			}
			return roles;
        }

		OptionListItem OptionsListModel::createOption(model::GwtOptionModel &item, std::string lineId, int hi, int lo)
        {
			OptionListItem inserted;
			inserted.lineId = lineId.c_str();
			inserted.assetId = item.assetId();
			inserted.name = item.name().c_str();
			inserted.moneyBack = item.moneyBack();
			inserted.rateLow = lo;
			inserted.rateHi = hi;
			inserted.price = item.price();
			if (item.seconds() == 30) inserted.option30 = item.optionId();
			if (item.seconds() == 60) inserted.option60 = item.optionId();
			if (item.seconds() == 120) inserted.option120 = item.optionId();
			if (item.seconds() == 300) inserted.option300 = item.optionId();
			return inserted;
        }

        void OptionsListModel::updateOptions(const std::vector<model::GwtOptionModel> &options)
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
	            auto hi = item.rate("Put");
	            auto lo = item.rate("Call");
				if (hi == 0 && lo == 0) {
					hi = 50;
					lo = 50;
				}

				std::stringstream linestream;
				linestream << std::setw(4) << std::setfill('0') << item.marketId() << "." 
					<< std::setw(6) << std::setfill('0') << item.assetId();

				auto lineId = linestream.str();

                for (auto i = 0; i < _items.size(); ++i) {
                    auto &current = _items[i];
                    if (item.assetId() == current.assetId) {
						added = true;
						auto roles = updateOption(current, item, hi, lo);
						if (roles.size() > 0) {
							dataChanged(index(i, 0), index(i, 0), roles);
						}
						break;
                    }
                    else if (lineId.c_str() < current.lineId) {
						added = true;
						auto inserted = createOption(item, lineId, hi, lo);
                        beginInsertRows(QModelIndex(), i, i);
                        _items.insert(i, inserted);
                        endInsertRows();
						break;
                    }
                }
                if (!added)
				{
					auto inserted = createOption(item, lineId, hi, lo);
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
    }
}