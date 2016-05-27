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

			void setOptionIndex(int index);
			int optionIndex() const;

            void setExpiryDate(const QDateTime &expiryDate);
            const QDateTime &expiryDate() const;

			void setBet(int64_t bet);
			int64_t bet() const;

            int64_t returned() const;
			void close(int64_t returned);
	        bool isOpened() const;
	        bool isWon() const;
	        bool isLoose() const;
			bool isParity() const;
        private:

            int64_t _transactionId;
            int64_t _optionId;
            const AssetModel *_asset;
			int _optionIndex;
            QDateTime _expiryDate;
			int64_t _bet;
            int64_t _returned;
			bool _opened;
        };

        inline int64_t TransactionModel::transactionId() const { return _transactionId; }

        inline int64_t TransactionModel::optionId() const { return _optionId; }

        inline const AssetModel &TransactionModel::asset() const { return *_asset; }

		inline int TransactionModel::optionIndex() const { return _optionIndex; }

        inline const QDateTime &TransactionModel::expiryDate() const { return _expiryDate; }

		inline int64_t TransactionModel::bet() const { return _bet; }

        inline int64_t TransactionModel::returned() const { return _returned; }

		inline bool TransactionModel::isOpened() const { return _opened; }

		inline bool TransactionModel::isWon() const { return _returned > _bet; }

		inline bool TransactionModel::isLoose() const { return _returned == 0; }

		inline bool TransactionModel::isParity() const { return _returned == _bet; }
	}
}


#endif //VANTAGEFX_TRANSACTIONMODEL_H
