//
// Created by AMakarenya on 10.05.2016.
//

#include "VantageFxModel.h"
#include "../api/GwtValue.h"

namespace vantagefx {
    namespace model {
        using namespace api;

        void VantageFxModel::setLut(api::GwtObjectPtr lut) {
            _lut = lut;
			_openId = _lut->item("lutTypes/[name='OptionStatus']/luts/[name='Open']/id");
			for (auto &rate : lut->query("lutTypes/[name='PositionType']/luts/*")) {
				auto rateLut = rate.value.toObject();
				_rates[QString::fromStdString(rateLut->value("name").toString())] = rateLut->value("id").toInt();
			}
		}

        int VantageFxModel::instrumentTypeId() const {
            auto typeSuper = _lut->item("lutTypes/[name='InstrumentTypeSuper']/luts/[name='ShortTerm']/id");
            return _lut->item("superRels/[instrumentTypeSuperId={0}]/instrumentTypeId", { typeSuper }).toInt();
        }

        QStringList VantageFxModel::servers() const {
            QStringList servers;
            for (auto &server: _lut->query("servers/*/key()")) {
                servers.push_back(QString::fromStdString(server.value.toString()));
            }
            return servers;
        }

        int VantageFxModel::rateId(QString name)
        {
            return _rates[name];
        }

        std::vector<GwtOptionModel> VantageFxModel::options() const {
            std::vector<GwtOptionModel> result;
            for (auto &opt : _refresh->query("options/[optionStatus={0}]", { _openId })) {
                auto obj = opt.value.toObject();
                auto id = obj->value("id").toLong();
                auto found = std::find_if(result.begin(), result.end(), [id](GwtOptionModel &option) {
                    return option.optionId() == id;
                });
                if (found != result.end()) loadOption(opt.value, *found);
                else {
                    result.push_back(GwtOptionModel());
                    loadOption(opt.value, result[result.size() - 1]);
                }
            }
            return result;
        }

        void VantageFxModel::loadOption(const api::GwtValue &value, GwtOptionModel &model) const {
            auto obj = value.toObject();
            auto id = obj->value("id").toInt();
            auto asset = obj->value("assetId");
            model.setOptionId(id);
            model.setAssetId(asset.toInt());
            model.setMoneyBack(obj->value("return").toInt());
            model.setSeconds(obj->value("optionSeconds").toInt());
            model.setReturnValue(obj->value("return").toInt());
            model.setName(_lut->item("assets/[id={0}]/name", { asset }).toString());
            model.setPrice(_refresh->item("assetUpdates/[assetId={0}]/targetPrice", { asset }).toDouble());
            auto subMarket = _lut->item("assets/[id={0}]/subMarketId", { asset });
            auto marketName = _lut->item("markets/[subMarkets/ * /id = {0}]/name", { subMarket }).toString();
            auto marketId = _lut->item("markets/[subMarkets/ * /id = {0}]/id", { subMarket }).toInt();
            auto subMarketName = _lut->item("markets/ * /subMarkets/[id = {0}]/name", { subMarket }).toString();
            model.setMarketId(marketId);
            model.setMarket(marketName);
            model.setSubMarket(subMarketName);
            model.setClose(obj->item("closeDate/value").toLong());
            auto rates = _refresh->item("positionsSentimentDto/map/[int()={0}]", { asset }).toObject();
            if (rates) {
				for (auto it = _rates.begin(); it != _rates.end(); ++it) {
                    auto rate = rates->item("[int() = {0}]/value", { GwtValue(*it) });
                    if (!rate.empty()) {
                        model.setRate(it.key().toStdString(), rate.toInt());
                    }
                }
            }
        }

	    GwtOptionModel VantageFxModel::optionInfo(int64_t optionId) const
	    {
			model::GwtOptionModel result;
			loadOption(_refresh->item("options/[id={0}]", { GwtValue(optionId) }), result);
			return result;
		}

	    void VantageFxModel::setAccount(api::GwtObjectPtr auth)
        {
			_auth = auth;
        }

	    int64_t VantageFxModel::accountId() const
	    {
			return _auth->item("account/accountId").toLong();
        }

	    bool VantageFxModel::isLoggedIn() const
	    {
			if (!_auth) return false;
			return !_auth->item("account/accountId").empty();
        }

	    QString VantageFxModel::userName() const
	    {
			return QString::fromStdString(_auth->item("account/fullName").toString());
		}

	    void VantageFxModel::updateOptions(api::GwtObjectPtr refresh)
        {
			_refresh = refresh;
        }

	    int VantageFxModel::assetId(int64_t optionId) const
	    {
			return _refresh->item("options/[id={0}]/assetId", { GwtValue(optionId) }).toInt();
        }

	    int64_t VantageFxModel::currentPrice(int assetId) const
	    {
			return _refresh->item("assetUpdates/[assetId={0}]/targetPrice", { GwtValue(assetId) }).toDouble();
        }

	    int64_t VantageFxModel::currentMoney() const
	    {
			return _refresh->item("money/value").toLong();
		}
    }
}

