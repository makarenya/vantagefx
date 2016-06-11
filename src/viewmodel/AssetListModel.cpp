//
// Created by AMakarenya on 07.06.2016.
//

#include "AssetListModel.h"

namespace vantagefx {
    namespace viewmodel {
	    AssetListModel::AssetListModel(QObject *parent)
                : QAbstractListModel(parent)
        {}

		QHash<int, QByteArray> AssetListModel::roleNames() const
        {
            QHash<int, QByteArray> roles;
            roles[Qt::DisplayRole] = "display";
            roles[AssetIdRole] = "asset";
            roles[NameRole] = "name";
            roles[RateLowRole] = "low";
            roles[RateHiRole] = "high";
            roles[PriceRole] = "price";
            roles[ThresholdRole] = "threshold";
			roles[OptionsRole] = "options";
			return roles;
        }

        QVariant AssetListModel::data(const QModelIndex &index, int role) const
        {
            if (!index.isValid()) return QVariant();
            if (index.row() > _items.size()) return QVariant();

            auto option = _items[index.row()];

            switch(role) {
                case Qt::DisplayRole:
                case NameRole:
                    return option->name();
                case AssetIdRole:
                    return option->assetId();
                case RateLowRole:
                    return option->rateLow();
                case RateHiRole:
                    return option->rateHi();
                case PriceRole:
                    return option->price();
				case ThresholdRole:
                    return option->threshold();
				case OptionsRole:
					return QVariant::fromValue(option);
				default:
                    return QVariant();
            }
        }

        bool AssetListModel::setData(const QModelIndex &index, const QVariant &value, int role)
        {
            if (!index.isValid()) return false;
            if (index.row() > _items.size()) return false;
            if (role != ThresholdRole) return false;
            _items[index.row()]->setThreshold(value.toInt());
            return true;
        }

		/**
         * @brief Добавляет пару к торгу
         */
		void AssetListModel::watchForAsset(const model::AssetModel &asset)
		{
            // Создание новой модели отображения пары и установка её параметров
            auto item = new OptionsListModel(this);
            item->setMarketId(asset.marketId());
            item->setAssetId(asset.id());
            item->setName(asset.name());
            item->setRateLow(asset.rateValue("Put"));
            item->setRateHi(asset.rateValue("Call"));
			item->setAllRates(asset.rates());
            item->setPrice(asset.price());

            // Все пары упорядочены по своему индексу, опредемому полем order.
            // Значение order осдержит идентификатор пары, так что он уникален
            for(int i = 0; i < _items.size(); ++i) {
                if (item->order() < _items[i]->order()) {
                    beginInsertRows(QModelIndex(), i, i);
                    _items.insert(i, item);
                    endInsertRows();
                    return;
                }
            }
			beginInsertRows(QModelIndex(), _items.size(), _items.size());
            _items.push_back(item);
			endInsertRows();
		}

        void AssetListModel::stopWatch(int assetId)
        {
            for(int i = 0; i < _items.size(); ++i) {
                if (_items[i]->assetId() == assetId) {
                    beginRemoveRows(QModelIndex(), i, i);
                    _items.removeAt(i);
                    endRemoveRows();
                    return;
                }
            }
        }

        int AssetListModel::rowCount(const QModelIndex &parent) const
        {
            auto cnt = _items.size();
			return cnt;
        }

        void AssetListModel::updateAssets(const QMap<int, model::AssetModel> &assets)
        {
            // Если опций нет, то вызов холостой.
            if (assets.empty()) return;

            // Перебор всей коллекции выбранных объектов
            for(int i = 0; i < _items.size(); ++i) {
                // Ссыль на текущий объект
                auto &current = _items[i];
                // Если в массиве переданных ассетов есть нужный
                if (assets.contains(current->assetId())) {
                    auto &item = assets[current->assetId()];

                    QVector<int> roles;
                    // Обновляются отображаемые данные
                    if (item.rateValue("Call") != current->rateLow()) {
                        current->setRateLow(item.rateValue("Call"));
                        roles.push_back(RateLowRole);
                    }
                    if (item.rateValue("Put") != current->rateHi()) {
                        current->setRateHi(item.rateValue("Put"));
                        roles.push_back(RateHiRole);
                    }
					current->setAllRates(item.rates());
                    if (item.price() != current->price()) {
                        current->setPrice(item.price());
                        roles.push_back(PriceRole);
                    }
                    // Если что-то обновлено, то это нужно отразить в UI
                    if (roles.size() > 0) {
                        dataChanged(index(i), index(i), roles);
                    }
                }
            }
        }

        void AssetListModel::updateOptions(const QMap<int64_t, model::OptionModel> &options)
        {
            // Если опций нет, то вызов холостой.
			if (options.empty()) return;

            // Собирается список всех опций. Найденные опции будут из него удаляться.
            // Оставшиеся будут подлежать удалению.
			QSet<int64_t> removed;
			for (auto &current : _items) {
				current->ids(removed);
			}

            // Если какая-то пара не отслеживается, то она автоматически добавляется
            for(auto &item : options) {
                if (_items.end() == std::find_if(_items.begin(), _items.end(),
                    [&item](OptionsListModel * model) { return model->assetId() == item.asset().id(); }))
                {
                    watchForAsset(item.asset());
                }
            }

            // Перебираются все переданные опции
            for(auto &item : options) {
                // Перебираются все пары из имеющихся
                for (auto i = 0; i < _items.size(); ++i) {
                    // Текуцая пара
                    auto &current = _items[i];

                    // Если текущая опция принадлежит к данной паре
                    if (item.asset().id() == current->assetId()) {
                        // Опция убирается из списка "на удаление"
                        removed.remove(item.optionId());
                        // Обновляются данные опции
                        current->updateOption(item);
						break;
                    }
                }
			}

            // Очистка исчезнувших опций
            for (auto i = 0; i < _items.size(); ++i) {
                auto &current = _items[i];
				current->remove(removed);
            }
        }

		void AssetListModel::updateOption(model::OptionModel &item)
		{
			for (auto i = 0; i < _items.size(); ++i) {
				if (_items[i]->contains(item.optionId()))
				{
					_items[i]->updateOption(item);
				}
			}
		}

        QMap<QString, QList<VirtualBet>> AssetListModel::calculateVirtualBets()
        {
            QMap<QString, QList<VirtualBet>> result;
            for(int i = 0; i < _items.size(); ++i) {
                result[_items[i]->name()] = _items[i]->calculateVirtualBets();
            }
            return result;
        }

		bool AssetListModel::containsOption(int64_t optionId) const
		{
			for (auto asset : _items) {
				if (asset->contains(optionId)) return true;
			}
			return false;
		}

		OptionsListModel &AssetListModel::assetFor(int64_t optionId) const
		{
			for (auto asset : _items) {
				if (asset->contains(optionId)) return *asset;
			}
			throw std::out_of_range("option " + std::to_string(optionId) + " not found");
		}

		bool AssetListModel::isSelected(int64_t optionId) const
		{
			return assetFor(optionId).isSelected(optionId);
		}

		int AssetListModel::threshold(int assetId) {
            for(auto item : _items) {
                if (item->assetId() == assetId) return item->threshold();
            }
            return 0;
        }
    }
}