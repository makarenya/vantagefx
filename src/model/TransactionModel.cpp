//
// Created by AMakarenya on 12.05.2016.
//

#include "TransactionModel.h"

namespace vantagefx {
    namespace model {

        TransactionModel::TransactionModel()
                : _transactionId(0),
                  _optionId(0),
                  _asset(nullptr), 
			      _bet(0),
			      _returned(0),
			      _opened(true)
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

		void TransactionModel::setOptionSeconds(int seconds)
		{
			_optionSeconds = seconds;
		}


        void TransactionModel::setExpiryDate(const QDateTime &expiryDate)
        {
            _expiryDate = expiryDate;
        }

		void TransactionModel::setBet(int64_t bet)
        {
			_bet = bet;
        }

	    void TransactionModel::close(int64_t returned)
        {
			_returned = returned;
			_opened = false;
        }
    }
}

