//
// Created by alexx on 27.04.2016.
//

#include "GwtOptionModel.h"

namespace vantagefx {
    namespace model {
        void GwtOptionModel::setOptionId(int optionId) {
            _optionId = optionId;
        }

        void GwtOptionModel::setAssetId(int assetId) {
            _assetId = assetId;
        }

        void GwtOptionModel::setName(const QString &name) {
            _name = name;
        }

        void GwtOptionModel::setMoneyBack(int moneyBack) {
            _moneyBack = moneyBack;
        }

        void GwtOptionModel::setSeconds(int seconds) {
            _seconds = seconds;
        }

        void GwtOptionModel::setRateLow(int rateLow) {
            _rateLow = rateLow;
        }

        void GwtOptionModel::setPrice(double price) {
            _price = price;
        }

        void GwtOptionModel::setClose(const QDateTime &close) {
            _close = close;
        }

        void GwtOptionModel::setRate(const QString &name, int value) {
            _rates[name] = value;
        }
    }
}