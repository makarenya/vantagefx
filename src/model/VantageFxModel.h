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
            int rateId(QString name);

			void setAccount(api::GwtObjectPtr auth);
			int64_t accountId() const;
			bool isLoggedIn() const;
			const QString &userName() const;

			void updateOptions(api::GwtObjectPtr refresh);
			const QMap<int64_t, OptionModel> &options() const;
			OptionModel &optionInfo(int64_t optionId);
			int64_t currentMoney() const;
	        void updatePurchase(api::GwtObjectPtr transaction);
	        void flushTransactions();
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
			QList<TransactionModel> _closedTransactions;

			int64_t _currentMoney;
        };
    }
}


#endif //VANTAGEFX_VANTAGEFXMODEL_H
