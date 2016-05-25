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
                  bet(0), 
				  seconds(0)
                {}

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
                    return QColor(Qt::white);
                case model::OptionModel::Selected:
                    return QColor("#e4f0fa");
                case model::OptionModel::Processing:
					return QColor("#565656");
                case model::OptionModel::Successful:
					return QColor("#5bff5b");
                case model::OptionModel::Failed:
					return QColor("#ff2828");
                default:
					return QColor("#f7ff28");
            }
        }

		QColor OptionItem::border() const
		{
			switch (status) {
			case model::OptionModel::Idle:
				return QColor("#b4b4b5");
			case model::OptionModel::Selected:
				return QColor("#0076d4");
			case model::OptionModel::Processing:
				return QColor(Qt::black);
			case model::OptionModel::Successful:
				return QColor("#009f00");
			case model::OptionModel::Failed:
				return QColor("#810000");
			default:
				return QColor("#b6bd00");
			}
		}
		
		QColor OptionItem::foreground() const
		{
			switch (status) {
			case model::OptionModel::Idle:
				return QColor(Qt::black);
			case model::OptionModel::Selected:
				return QColor(Qt::black);
			case model::OptionModel::Processing:
				return QColor(Qt::white);
			case model::OptionModel::Successful:
				return QColor(Qt::black);
			case model::OptionModel::Failed:
				return QColor(Qt::white);
			default:
				return QColor(Qt::black);
			}
		}
		
		OptionsListModel::OptionsListModel(QObject *parent)
			: QAbstractListModel(parent),
			  _assetId(0),
			  _moneyBack(0),
			  _rateHi(0),
			  _rateLow(0),
			  _price(0.0),
			  _threshold(71),
			  _options({ OptionItem(), OptionItem(), OptionItem(), OptionItem() })
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
			}
			return QVariant();
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
			return result;
		}

		const OptionItem &OptionsListModel::option(int index) const
		{
			return _options[index];
		}

	    void OptionsListModel::updateOption(model::OptionModel &item)
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
					option.status = model::OptionModel::Idle;
					option.bet = 0;
					dataChanged(index(j), index(j), { IdRole, StatusRole, BackgroundRole, BorderRole, ForegroundRole, BetRole });
				}
				if (_options[j].id == 0) empty++;
			}
			return empty == _options.size();
        }

	    void OptionsListModel::setAssetId(int assetId)
        {
	        _assetId = assetId;
        }

	    void OptionsListModel::setName(QString name)
        {
	        _name = name;
        }

	    void OptionsListModel::setMoneyBack(int moneyBack)
        {
	        _moneyBack = moneyBack;
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

	    void OptionsListModel::setMarket(QString market)
        {
	        _market = market;
        }

	    void OptionsListModel::setSubMarket(QString subMarket)
        {
	        _subMarket = subMarket;
        }

	    void OptionsListModel::setLineId(QString lineId)
        {
	        _lineId = lineId;
        }

	    void OptionsListModel::setThreshold(int threshold)
        {
	        _threshold = threshold;
        }

	    AssetListModel::AssetListModel(QObject *parent)
                : QAbstractListModel(parent), 
			      _options(nullptr)
        {}
		
		QHash<int, QByteArray> AssetListModel::roleNames() const
        {
            QHash<int, QByteArray> roles;
            roles[Qt::DisplayRole] = "display";
            roles[AssetIdRole] = "asset";
            roles[NameRole] = "name";
            roles[MoneyBackRole] = "back";
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
                case MoneyBackRole:
                    return option->moneyBack();
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

		void AssetListModel::select(long long optionId)
		{
			auto &option = (*_options)[optionId];
			option.toggle();
			updateOption(option);
		}

        int AssetListModel::rowCount(const QModelIndex &parent) const
        {
            auto cnt = _items.size();
			return cnt;
        }

		QVector<int> AssetListModel::updateOption(OptionsListModel *current, model::OptionModel &item)
        {
			QVector<int> roles;
			if (item.asset().name() != current->name()) {
				current->setName(item.asset().name());
				roles.push_back(NameRole);
			}
			if (item.moneyBack() != current->moneyBack()) {
				current->setMoneyBack(item.moneyBack());
				roles.push_back(MoneyBackRole);
			}
			if (item.lowRateValue() != current->rateLow()) {
				current->setRateLow(item.lowRateValue());
				roles.push_back(RateLowRole);
			}
			if (item.highRateValue() != current->rateHi()) {
				current->setRateHi(item.highRateValue());
				roles.push_back(RateHiRole);
			}
			if (item.price() != current->price()) {
				current->setPrice(item.price());
				roles.push_back(PriceRole);
			}
			current->updateOption(item);
			return roles;
        }

		OptionsListModel *AssetListModel::createOption(model::OptionModel &item, std::string lineId)
        {
	        auto inserted = new OptionsListModel(this);
			inserted->setLineId(lineId.c_str());
			inserted->setAssetId(item.asset().id());
			inserted->setName(item.asset().name());
			inserted->setMoneyBack(item.moneyBack());
			inserted->setRateLow(item.lowRateValue());
			inserted->setRateHi(item.highRateValue());
			inserted->setPrice(item.price());
			inserted->updateOption(item);
			return inserted;
        }

        void AssetListModel::updateOptions(QMap<int64_t, model::OptionModel> &options)
        {
			_options = &options;
			if (options.empty()) return;
			QSet<int64_t> removed;
			for (auto &current : _items) {
				current->ids(removed);
			}
			for(auto &item: options) {
	            auto added = false;
				std::stringstream linestream;
				linestream << std::setw(4) << std::setfill('0') << item.asset().marketId() << "." 
					<< std::setw(6) << std::setfill('0') << item.asset().id();

				auto lineId = linestream.str();

                for (auto i = 0; i < _items.size(); ++i) {
                    auto &current = _items[i];
                    if (item.asset().id() == current->assetId()) {
						added = true;
						auto roles = updateOption(current, item);
						removed.remove(item.optionId());
						if (roles.size() > 0) {
							dataChanged(index(i, 0), index(i, 0), roles);
						}
						break;
                    }
                    else if (lineId.c_str() < current->lineId()) {
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
				if (current->remove(removed)) {
					beginRemoveRows(QModelIndex(), i, i);
					delete _items[i];
					_items.removeAt(i--);
					endRemoveRows();
				}
                else if (roles.size() > 0) {
                    dataChanged(index(i, 0), index(i, 0), roles);
                }
            }
        }

		void AssetListModel::updateOption(model::OptionModel &item)
		{
			for (auto i = 0; i < _items.size(); ++i) {
				if (_items[i]->option(item.index()).id == item.optionId())
				{
					_items[i]->updateOption(item);
				}
			}
		}

		int AssetListModel::threshold(int assetId) {
            for(auto item : _items) {
                if (item->assetId() == assetId) return item->threshold();
            }
            return 0;
        }
	}
}