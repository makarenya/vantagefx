//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_GWTOPTIONMODEL_H
#define VANTAGEFX_GWTOPTIONMODEL_H

#include <QtCore>
#include <map>
#include <string>
#include "AssetModel.h"


namespace vantagefx {
    namespace model {

        class OptionModel
        {
        public:

			enum OptionStatus {
				Idle,
				Selected,
				Processing,
				Successful,
				Failed,
				Returned
			};

			OptionModel();

			int64_t optionId() const;

            AssetModel &asset() const;

            int moneyBack() const;

            int seconds() const;

			int returnValue() const;

            double price() const;

			int lowRateValue() const;

			int highRateValue() const;

			QDateTime close() const;

            void setOptionId(int64_t optionId);

            void setAsset(AssetModel *assetId);

            void setMoneyBack(int moneyBack);

            void setSeconds(int seconds);

			void setReturnValue(int returnValue);

            void setPrice(double price);

            void setClose(QDateTime close);

			OptionStatus status() const;

			void setChecked(bool checked);
            bool checked() const;

			void openTransaction();
			void closeSuccess();
			void closeFail();
			void closeReturn();
		
		private:
            int64_t _optionId;
			AssetModel *_asset;
            int _moneyBack;
            int _seconds;
			int _returnValue;
            double _price;
            QDateTime _close;
            bool _checked;
			OptionStatus _status;
			QDateTime _openTime;
        };

        inline QDateTime OptionModel::close() const { return _close; }

	    inline double OptionModel::price() const { return _price; }

        inline int OptionModel::seconds() const { return _seconds; }

		inline int OptionModel::returnValue() const { return _returnValue; }

        inline int OptionModel::moneyBack() const { return _moneyBack; }

        inline AssetModel &OptionModel::asset() const { return *_asset; }

        inline int64_t OptionModel::optionId() const { return _optionId; }

        inline bool OptionModel::checked() const { return _checked; }
    }
}


#endif //VANTAGEFX_GWTOPTIONMODEL_H
