//
// Created by alexx on 27.04.2016.
//

#include "GwtOptionModel.h"

namespace vantagefx {
    namespace model {

	    GwtOptionModel::GwtOptionModel()
				: _optionId(0),
	              _assetId(0),
	              _moneyBack(0),
	              _seconds(0),
	              _returnValue(0),
	              _rateLow(0),
	              _price(0),
	              _marketId(0),
	              _close(0)
	    {}

	    int GwtOptionModel::rate(const std::string &name) const
		{
			auto it = _rates.find(name);
			if (it == _rates.end()) return 0;
			return it->second;
		}

		void GwtOptionModel::setOptionId(int64_t optionId) {
            _optionId = optionId;
        }

        void GwtOptionModel::setAssetId(int assetId) {
            _assetId = assetId;
        }

        void GwtOptionModel::setName(const std::string &name) {
            _name = name;
        }

        void GwtOptionModel::setMoneyBack(int moneyBack) {
            _moneyBack = moneyBack;
        }

        void GwtOptionModel::setSeconds(int seconds) {
            _seconds = seconds;
        }

        void GwtOptionModel::setReturnValue(int returnValue) {
            _returnValue = returnValue;
        }

        void GwtOptionModel::setRateLow(int rateLow) {
            _rateLow = rateLow;
        }

        void GwtOptionModel::setPrice(double price) {
            _price = price;
        }

		void GwtOptionModel::setMarketId(int marketId) {
			_marketId = marketId;
		}
		
		void GwtOptionModel::setMarket(const std::string &market) {
            _market = market;
        }

        void GwtOptionModel::setSubMarket(const std::string &subMarket) {
            _subMarket = subMarket;
        }

        void GwtOptionModel::setClose(int64_t close) {
            _close = close;
        }

        void GwtOptionModel::setRate(const std::string &name, int value) {
            _rates[name] = value;
        }
    }
}