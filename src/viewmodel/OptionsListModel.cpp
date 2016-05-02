//
// Created by alexx on 27.04.2016.
//

#include "OptionsListModel.h"


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

        void OptionsListModel::updateOptions(std::map<int, model::GwtOptionModel> options)
        {
            for(auto pair: options) {
				auto &item = pair.second;
	            auto added = false;
	            auto empty = item.rate("Put") == 0 || item.rate("Call") == 0;

                for (auto &current : _items) {
                    current.option30 = -current.option30;
                    current.option60 = -current.option60;
                    current.option120 = -current.option120;
                    current.option300 = -current.option300;
                }

                for (auto i = 0; i < _items.size(); ++i) {
                    auto &current = _items[i];
                    if (item.assetId() == current.assetId) {
						added = true;
                        QVector<int> roles;
                        if (item.name() != current.name) {
                            current.name = item.name();
                            roles.push_back(NameRole);
                        }
                        if (item.moneyBack() != current.moneyBack) {
                            current.moneyBack = item.moneyBack();
                            roles.push_back(MoneyBackRole);
                        }
                        if (item.rate("Call") != current.rateLow) {
                            current.rateLow = item.rate("Call");
                            roles.push_back(RateLowRole);
                        }
                        if (item.rate("Put") != current.rateHi) {
                            current.rateHi = item.rate("Put");
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

						if (empty) {
							beginRemoveRows(QModelIndex(), i, i);
							_items.removeAt(i);
							endRemoveRows();
						}
						else if (roles.size() > 0) {
                            dataChanged(index(i, 0), index(i, 0), roles);
                        }
                        break;
                    }
                    else if (item.assetId() < current.assetId && !empty) {
						added = true;
                        OptionListItem inserted;
						inserted.assetId = item.assetId();
                        inserted.name = item.name();
                        inserted.moneyBack = item.moneyBack();
                        inserted.rateLow = item.rate("Call");
                        inserted.rateHi = item.rate("Put");
                        inserted.price = item.price();
                        beginInsertRows(QModelIndex(), i, i);
                        _items.insert(i, inserted);
                        endInsertRows();
						break;
                    }
                }
                if (!added && !empty)
				{
                    OptionListItem inserted;
                    inserted.assetId = item.assetId();
                    inserted.name = item.name();
                    inserted.moneyBack = item.moneyBack();
                    inserted.rateLow = item.rate("Call");
                    inserted.rateHi = item.rate("Put");
                    inserted.price = item.price();
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
                if (roles.size() > 0) {
                    dataChanged(index(i, 0), index(i, 0), roles);
                }
            }
        }
    }
}