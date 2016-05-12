//
// Created by AMakarenya on 12.05.2016.
//

#ifndef VANTAGEFX_TRANSACTIONMODEL_H
#define VANTAGEFX_TRANSACTIONMODEL_H

#include "AssetModel.h"

namespace vantagefx {
    namespace model {

        class TransactionModel {
        public:
            TransactionModel();

            void setTransactionId(int64_t transactionId);
            int64_t transactionId() const;

            void setOptionId(int64_t optionId);
            int64_t optionId() const;

            void setAsset(const AssetModel &asset);
            const AssetModel & asset() const;

            void setExpiryDate(const QDateTime &expiryDate);
            const QDateTime &expiryDate() const;

            void setReturned(int64_t returned);
            int64_t returned() const;

        private:

            int64_t _transactionId;
            int64_t _optionId;
            const AssetModel *_asset;
            QDateTime _expiryDate;
            int64_t _returned;
        };

        inline int64_t TransactionModel::transactionId() const { return _transactionId; }

        inline int64_t TransactionModel::optionId() const { return _optionId; }

        inline const AssetModel &TransactionModel::asset() const { return *_asset; }

        inline const QDateTime &TransactionModel::expiryDate() const { return _expiryDate; }

        inline int64_t TransactionModel::returned() const { return _returned; }
    }
}


#endif //VANTAGEFX_TRANSACTIONMODEL_H
