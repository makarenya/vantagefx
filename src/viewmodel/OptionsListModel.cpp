//
// Created by alexx on 27.04.2016.
//

#include "OptionsListModel.h"
#include <sstream>
#include <iomanip>


namespace vantagefx {
    namespace viewmodel {

        OptionItem::OptionItem(int s)
                : id(0),
                  status(model::OptionModel::NotFound),
                  bet(0), 
				  seconds(s),
				  selected(false)
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
		
		OptionsListModel::OptionsListModel(QObject *parent)
			: QAbstractListModel(parent),
			  _assetId(0),
			  _rateHi(0),
			  _rateLow(0),
			  _price(0.0),
			  _threshold(71),
			  _options({ OptionItem(30), OptionItem(60), OptionItem(120), OptionItem(300) })
        {}

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