//
// Created by AMakarenya on 10.05.2016.
//

#ifndef VANTAGEFX_VANTAGEFXMODEL_H
#define VANTAGEFX_VANTAGEFXMODEL_H

#include <QtCore>
#include "../api/GwtObject.h"
#include "OptionModel.h"
#include "AssetModel.h"
#include "TransactionModel.h"

namespace vantagefx {
    namespace model {

        class VantageFxModel {
        public:

			VantageFxModel();
			
			void setLut(api::GwtObjectPtr lut);
            int instrumentTypeId() const;
			const QStringList &servers() const;
			const QMap<int, AssetModel> &assets() const;
            int rateId(QString name);

			void setAccount(api::GwtObjectPtr auth);
			int64_t accountId() const;
			bool isLoggedIn() const;
			const QString &userName() const;

			void setBetAmount(int betAmount);
			int betAmount() const;
			void setBetsCount(int betsCount);
			int betsCount() const;

			QList<TransactionModel> updateOptions(api::GwtObjectPtr refresh);
			QMap<int64_t, OptionModel> &options();
			OptionModel &optionInfo(int64_t optionId);
			AssetModel &assetInfo(int assetId);
			int64_t currentMoney() const;
	        std::tuple<OptionModel &, TransactionModel &> updatePurchase(api::GwtObjectPtr transaction);
			bool hasTransactionFor(int64_t optionId) const;

        private:

			int _openId;
            int _positionClosed;
			int _instrumentTypeId;
			QStringList _servers;
			QMap<QString, int> _rates;
			QMap<int, AssetModel> _assets;

			int64_t _accountId;
			QString _userName;

			QMap<int64_t, OptionModel> _options;
			QMap<int64_t, TransactionModel> _openedTransactions;
			int64_t _currentMoney;

			int _betAmount;
			int _betsCount;
        };
    }
}


#endif //VANTAGEFX_VANTAGEFXMODEL_H
