//
// Created by alexx on 27.04.2016.
//

#include "OptionsListModel.h"


namespace vantagefx {
    namespace viewmodel {

        OptionsListModel::OptionsListModel(QObject *parent)
                : QAbstractListModel(parent)
        {
            connect(this, &OptionsListModel::updateOptions, &OptionsListModel::optionsUpdate);
        }

        QHash<int, QByteArray> OptionsListModel::roleNames() const
        {
            QHash<int, QByteArray> roles;
            roles[Qt::DisplayRole] = "display";
            roles[OptionIdRole] = "id";
            roles[AssetIdRole] = "asset";
            roles[NameRole] = "name";
            roles[MoneyBackRole] = "back";
            roles[RateLowRole] = "low";
            roles[RateHiRole] = "high";
            roles[PriceRole] = "price";
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
                    return option.name();
                case OptionIdRole:
                    return option.optionId();
                case AssetIdRole:
                    return option.assetId();
                case MoneyBackRole:
                    return option.moneyBack();
                case RateLowRole:
                    return option.rate("Call");
                case RateHiRole:
                    return option.rate("Put");
                case PriceRole:
                    return option.price();
                default:
                    return QVariant();
            }
        }

        int OptionsListModel::rowCount(const QModelIndex &parent) const
        {
            auto cnt = _items.size();
			return cnt;
        }

        void OptionsListModel::optionsUpdate(QMap<int, model::GwtOptionModel> options)
        {
            for(auto item: options) {
	            auto added = false;
	            auto empty = item.rate("Put") == 0 || item.rate("Call") == 0;
                for(auto i = 0; i < _items.size(); ++i) {
                    auto &current = _items[i];
                    if (item.assetId() == current.assetId()) {
						added = true;
                        QVector<int> roles;
                        if (item.name() != current.name()) {
                            current.setName(item.name());
                            roles.push_back(NameRole);
                        }
                        if (item.moneyBack() != current.moneyBack()) {
                            current.setMoneyBack(item.moneyBack());
                            roles.push_back(MoneyBackRole);
                        }
                        if (item.rate("Call") != current.rate("Call")) {
                            current.setRate("Call", item.rate("Call"));
                            roles.push_back(RateLowRole);
                        }
                        if (item.rate("Put") != current.rate("Put")) {
                            current.setRate("Put", item.rate("Put"));
                            roles.push_back(RateHiRole);
                        }
                        if (item.price() != current.price()) {
                            current.setPrice(item.price());
                            roles.push_back(PriceRole);
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
                    else if (item.assetId() < current.assetId() && !empty) {
						added = true;
						model::GwtOptionModel inserted;
						inserted.setAssetId(item.assetId());
                        inserted.setName(item.name());
                        inserted.setMoneyBack(item.moneyBack());
                        inserted.setRate("Call", item.rate("Call"));
                        inserted.setRate("Put", item.rate("Put"));
                        inserted.setPrice(item.price());
                        inserted.setPrice(item.price());
                        beginInsertRows(QModelIndex(), i, i);
                        _items.insert(i, inserted);
                        endInsertRows();
						break;
                    }
                }
				if (!added && !empty)
				{
					added = true;
					model::GwtOptionModel inserted;
					inserted.setAssetId(item.assetId());
					inserted.setName(item.name());
					inserted.setMoneyBack(item.moneyBack());
					inserted.setRate("Call", item.rate("Call"));
					inserted.setRate("Put", item.rate("Put"));
					inserted.setPrice(item.price());
					inserted.setPrice(item.price());
					beginInsertRows(QModelIndex(), _items.size(), _items.size());
					_items.push_back(inserted);
					endInsertRows();
				}
			}
        }
    }
}