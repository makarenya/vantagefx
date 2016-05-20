//
// Created by alexx on 27.04.2016.
//

#include "OptionsListModel.h"
#include <sstream>
#include <iomanip>


namespace vantagefx {
    namespace viewmodel {

        OptionItem::OptionItem()
                : id(0),
                  status(model::OptionModel::Idle),
                  bet(0)
        {}

        QColor OptionItem::color() const
        {
            switch (status) {
                case model::OptionModel::Idle:
                    return QColor(Qt::white);
                case model::OptionModel::Selected:
                    return QColor("#1882d7");
                case model::OptionModel::Processing:
                    return QColor(Qt::black);
                case model::OptionModel::Successful:
                    return QColor(Qt::green);
                case model::OptionModel::Failed:
                    return QColor(Qt::red);
                default:
                    return QColor(Qt::yellow);
            }
        }

		AssetListItem::AssetListItem()
				: assetId(0),
				  moneyBack(0),
				  rateHi(0),
				  rateLow(0),
				  price(0.0),
				  threshold(71),
                  options({ OptionItem(), OptionItem(), OptionItem(), OptionItem() })
		{}

        OptionsListModel::OptionsListModel(QObject *parent)
                : QAbstractListModel(parent), 
			      _options(nullptr)
        {}
		
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
                    return option.options[0].id;
                case Option60Role:
					return option.options[1].id;
				case Option120Role:
					return option.options[2].id;
				case Option300Role:
					return option.options[3].id;
				case ThresholdRole:
                    return option.threshold;
				case Color30Role:
					return option.options[0].color();
				case Color60Role:
					return option.options[1].color();
				case Color120Role:
					return option.options[2].color();
				case Color300Role:
					return option.options[3].color();
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
			for(auto i = 0; i < _items.size(); ++i) {
				auto &item = _items[i];
				for(auto j = 0; j < 4; ++j) {
					auto currentId = item.options[j].id;
					if (currentId == optionId) {
						auto &option = (*_options)[currentId];
						option.toggle();
						dataChanged(index(i), index(i), updateOption(item, option));
						return;
					}
				}
			}
		}

        int OptionsListModel::rowCount(const QModelIndex &parent) const
        {
            auto cnt = _items.size();
			return cnt;
        }

		QVector<int> OptionsListModel::updateOption(AssetListItem &current, model::OptionModel &item)
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
			if (item.optionId() != current.options[item.index()].id) {
				current.options[item.index()].id = item.optionId();
				roles.push_back(Option30Role + item.index());
			}
			if (item.status() != current.options[item.index()].status) {
				current.options[item.index()].status = item.status();
				roles.push_back(Color30Role + item.index());
			}
			return roles;
        }

		AssetListItem OptionsListModel::createOption(model::OptionModel &item, std::string lineId)
        {
			AssetListItem inserted;
			inserted.lineId = lineId.c_str();
			inserted.assetId = item.asset().id();
			inserted.name = item.asset().name();
			inserted.moneyBack = item.moneyBack();
			inserted.rateLow = item.lowRateValue();
			inserted.rateHi = item.highRateValue();
			inserted.price = item.price();
			inserted.options[item.index()].id = item.optionId();
            inserted.options[item.index()].status = item.status();
			return inserted;
        }

        void OptionsListModel::updateOptions(QMap<int64_t, model::OptionModel> &options)
        {
			_options = &options;
			if (options.empty()) return;
			QSet<int64_t> removed;
			for (auto &current : _items) {
				for (auto &item : current.options) {
					if (item.id != 0) removed.insert(item.id);
				}
			}
			for(auto &item: options) {
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
						removed.remove(item.optionId());
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
				auto empty = 0;
				for(auto j = 0; j < 4; ++j) {
					if (removed.contains(current.options[j].id)) {
						current.options[j].id = 0;
						roles.push_back(Option30Role + j);
					}
					if (current.options[j].id == 0) {
						empty++;
					}
				}
				
				if (empty == 4) {
					beginRemoveRows(QModelIndex(), i, i);
					_items.removeAt(i--);
					endRemoveRows();
				}
                else if (roles.size() > 0) {
                    dataChanged(index(i, 0), index(i, 0), roles);
                }
            }
        }

		void OptionsListModel::updateOption(model::OptionModel &item)
		{
			for (auto i = 0; i < _items.size(); ++i) {
				auto &current = _items[i];
				for (auto j = 0; j < 4; ++j) {
					if (current.options[item.index()].id == item.optionId()) {
						dataChanged(index(i), index(i), updateOption(current, item));
					}
				}
			}
		}

		int OptionsListModel::threshold(int assetId) {
            for(auto item : _items) {
                if (item.assetId == assetId) return item.threshold;
            }
            return 0;
        }

	}
}