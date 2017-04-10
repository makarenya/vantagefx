//
// Created by AMakarenya on 10.05.2016.
//

#include "VantageFxModel.h"
#include "../api/GwtValue.h"
#include "AssetModel.h"

namespace vantagefx {
    namespace model {
        using namespace api;

	    VantageFxModel::VantageFxModel()
				: _openId(0), 
			      _positionClosed(0),
	              _instrumentTypeId(0),
	              _accountId(0),
	              _currentMoney(0),
			      _betAmount(100),
			      _betsCount(1)
	    {}

	    void VantageFxModel::setLut(api::GwtObjectPtr lut) {
			for (auto &rate : lut->query("lutTypes/[name='PositionType']/luts/*")) {
				auto rateLut = rate.value.toObject();
				_rates[QString::fromStdString(rateLut->value("name").toString())] = rateLut->value("id").toInt();
			}
			auto typeSuper = lut->item("lutTypes/[name='InstrumentTypeSuper']/luts/[name='ShortTerm']/id");
			_positionClosed = lut->item("lutTypes/[name='PositionStatus']/luts/[name='Closed']/id").toInt();
			_instrumentTypeId = lut->item("superRels/[instrumentTypeSuperId={0}]/instrumentTypeId", { typeSuper }).toInt();

			_openId = lut->item("lutTypes/[name='OptionStatus']/luts/[name='Open']/id").toInt();

			_servers.clear();
			for (auto &server : lut->query("servers/*/key()")) {
				_servers.push_back(QString::fromStdString(server.value.toString()));
			}

			_options.clear();
			_assets.clear();

			for(auto &item : lut->query("assets/*")) {
				auto asset = item.value.toObject();
				auto id = asset->value("id").toInt();
				auto &result = _assets[id];
				result.setId(id);
				result.setName(QString::fromStdString(asset->value("name").toString()));

				auto subMarket = asset->value("subMarketId");
				auto marketName = lut->item("markets/[subMarkets/*/id={0}]/name", { subMarket }).toString();
				auto marketId = lut->item("markets/[subMarkets/*/id={0}]/id", { subMarket }).toInt();
				auto subMarketName = lut->item("markets/ * /subMarkets/[id = {0}]/name", { subMarket }).toString();
				result.setMarketId(marketId);
				result.setMarketName(QString::fromStdString(marketName));
				result.setSubMarketName(QString::fromStdString(subMarketName));
			}
		}

		int VantageFxModel::instrumentTypeId() const {
			return _instrumentTypeId;
		}

		const QStringList &VantageFxModel::servers() const {
			return _servers;
		}

		const QMap<int, AssetModel> &VantageFxModel::assets() const {
			return _assets;
		};

		int VantageFxModel::rateId(QString name)
		{
			return _rates[name];
		}

		void VantageFxModel::setBetAmount(int betAmount)
		{
			_betAmount = betAmount;
		}

		int VantageFxModel::betAmount() const {
			return _betAmount;
		}

		void VantageFxModel::setBetsCount(int betsCount) {
			_betsCount = betsCount;
		}

		int VantageFxModel::betsCount() const {
			return _betsCount;
		}

		QList<TransactionModel> VantageFxModel::updateOptions(api::GwtObjectPtr refresh)
		{
			_currentMoney = refresh->item("money/value").toLong();

			for(auto item : refresh->query("assetUpdates/*")) {
				auto update = item.value.toObject();
				auto id = update->value("assetId").toInt();
				auto price = update->value("targetPrice").toDouble();
				if (_assets.contains(id)) {
					_assets[id].setPrice(price);
				}
			}

			auto rates = refresh->item("positionsSentimentDto/map").toObject();
			if (rates) {
				for(auto pair : rates->values()) {
					auto id = std::stoi(pair.first);
					auto map = pair.second.toObject();
					if (!_assets.contains(id)) continue;
					auto &asset = _assets[id];
					for (auto it = _rates.begin(); it != _rates.end(); ++it) {
						auto rate = map->item("[int() = {0}]/value", { GwtValue(*it) });
						if (!rate.empty()) {
							asset.setRate(it.key(), rate.toInt());
						}
					}
				}
			}

			QSet<int64_t> old;
			for (auto it = _options.begin(); it != _options.end(); ++it) {
				old.insert(it.key());
			}
			for (auto &opt : refresh->query("options/[optionStatus={0}]", { GwtValue(_openId) })) {
				auto obj = opt.value.toObject();
				auto id = obj->value("id").toLong();
				auto asset = obj->value("assetId");
				if (!_assets.contains(asset.toInt())) continue;
				old.remove(id);
				auto &model = _options[id];
				model.setOptionId(id);
				model.setAsset(&_assets[asset.toInt()]);
				model.setSeconds(obj->value("optionSeconds").toInt());
			}
			for(auto id : old) {
				_options.remove(id);
			}

			QList<TransactionModel> result;

			for(auto &pos : refresh->query("positionUpdates/[positionStatus={0}]", { GwtValue(_positionClosed) })) {
				auto obj = pos.value.toObject();
				auto id = obj->value("transactionId").toLong();
				auto returned = obj->value("returned").toLong();
				if (!_openedTransactions.contains(id)) continue;
				auto transaction = std::move(_openedTransactions[id]);
                transaction.close(returned);
				_openedTransactions.remove(id);

				if (_options.contains(transaction.optionId())) {
                    auto &option = _options[transaction.optionId()];
					if (transaction.isWon()) {
						option.closeSuccess();
					}
					else if (transaction.isLoose()){
						option.closeFail();
					}
					else option.closeReturn();
                }

				result.push_back(std::move(transaction));
			}

			return result;
		}

		QMap<int64_t, OptionModel> &VantageFxModel::options() {
			return _options;
        }

	    OptionModel &VantageFxModel::optionInfo(int64_t optionId)
	    {
			return _options[optionId];
		}

        AssetModel &VantageFxModel::assetInfo(int assetId)
        {
            return _assets[assetId];
        }

	    void VantageFxModel::setAccount(api::GwtObjectPtr auth)
        {
			_accountId = auth->item("account/accountId").toLong();
			_userName = QString::fromStdString(auth->item("account/fullName").toString());
        }

	    int64_t VantageFxModel::accountId() const
	    {
			return _accountId;
        }

	    bool VantageFxModel::isLoggedIn() const
	    {
			return _accountId != 0;
        }

	    const QString &VantageFxModel::userName() const
	    {
			return _userName;
		}

	    int64_t VantageFxModel::currentMoney() const
	    {
			return _currentMoney;
		}

		std::tuple<OptionModel &, TransactionModel &> VantageFxModel::updatePurchase(api::GwtObjectPtr transaction)
        {
			auto optionId = transaction->item("option/id").toLong();
			auto transactionId = transaction->value("transactionId").toLong();
			auto bet = transaction->value("bet").toLong();
			auto &option = _options[optionId];
			auto expiryDate = QDateTime::currentDateTime().addSecs(option.seconds() + 15);
			auto &item = _openedTransactions[transactionId];
			item.setTransactionId(transactionId);
			item.setBet(bet);
			item.setOptionId(optionId);
			item.setExpiryDate(expiryDate);
			item.setAsset(_options[optionId].asset());
			item.setOptionIndex(option.index());
			option.openTransaction();
			return std::make_tuple(std::ref(option), std::ref(item));
		}


		bool VantageFxModel::hasTransactionFor(int64_t optionId) const
		{
			for(auto &transaction : _openedTransactions) {
                if (transaction.optionId() == optionId) return true;
            }
            return false;
		}
	}
}
