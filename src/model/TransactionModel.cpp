//
// Created by AMakarenya on 12.05.2016.
//

#include "TransactionModel.h"

namespace vantagefx {
    namespace model {

        TransactionModel::TransactionModel()
                : _transactionId(0),
                  _optionId(0),
                  _asset(nullptr)
        {}

        void TransactionModel::setTransactionId(int64_t transactionId)
        {
            _transactionId = transactionId;
        }

        void TransactionModel::setOptionId(int64_t optionId)
        {
            _optionId = optionId;
        }

        void TransactionModel::setAsset(const AssetModel &asset)
        {
            _asset = &asset;
        }

        void TransactionModel::setExpiryDate(const QDateTime &expiryDate)
        {
            _expiryDate = expiryDate;
        }

        void TransactionModel::setReturned(int64_t returned)
        {
            _returned = returned;
        }
    }
}

