//
// Created by AMakarenya on 10.05.2016.
//

#include "VantageFxModel.h"
#include "../api/GwtValue.h"
#include "AssetModel.h"

namespace vantagefx {
    namespace model {
        using namespace api;

        void VantageFxModel::setLut(api::GwtObjectPtr lut) {
			for (auto &rate : lut->query("lutTypes/[name='PositionType']/luts/*")) {
				auto rateLut = rate.value.toObject();
				_rates[QString::fromStdString(rateLut->value("name").toString())] = rateLut->value("id").toInt();
			}
			auto typeSuper = lut->item("lutTypes/[name='InstrumentTypeSuper']/luts/[name='ShortTerm']/id");
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

		int VantageFxModel::rateId(QString name)
		{
			return _rates[name];
		}

		void VantageFxModel::updateOptions(api::GwtObjectPtr refresh)
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


			for (auto &opt : refresh->query("options/[optionStatus={0}]", { GwtValue(_openId) })) {
				auto obj = opt.value.toObject();
				auto id = obj->value("id").toLong();
				auto &model = _options[id];
				auto asset = obj->value("assetId");
				if (!_assets.contains(asset.toInt())) continue;

				model.setOptionId(id);
				model.setAsset(&_assets[asset.toInt()]);
				model.setMoneyBack(obj->value("return").toInt());
				model.setSeconds(obj->value("optionSeconds").toInt());
				model.setReturnValue(obj->value("return").toInt());
				model.setClose(obj->item("closeDate/value").toLong());
			}
		}

		const QMap<int64_t, OptionModel> &VantageFxModel::options() const {
			return _options;
        }

	    OptionModel VantageFxModel::optionInfo(int64_t optionId) const
	    {
			return _options[optionId];
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
    }
}

