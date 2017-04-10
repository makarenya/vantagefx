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
				Processing,
				Successful,
				Failed,
				Returned,
				NotFound
			};

			OptionModel();

			int64_t optionId() const;

            AssetModel &asset() const;

            int seconds() const;

			int index() const;

            double price() const;

			int rate(const QString &name) const;

			QDateTime close() const;

            void setOptionId(int64_t optionId);

            void setAsset(AssetModel *assetId);

            void setSeconds(int seconds);

            void setPrice(double price);

            void setClose(QDateTime close);

			OptionStatus status() const;

			bool isAvailable() const;

			void openTransaction();
			void closeSuccess();
			void closeFail();
			void closeReturn();
		
		private:
            int64_t _optionId;
			AssetModel *_asset;
            int _seconds;
            double _price;
            QDateTime _close;
			OptionStatus _status;
			QDateTime _openTime;
        };

        inline QDateTime OptionModel::close() const { return _close; }

	    inline double OptionModel::price() const { return _price; }

        inline int OptionModel::seconds() const { return _seconds; }

		inline int OptionModel::index() const
        {
			switch(_seconds) {
			case 60: return 1;
			case 120: return 2;
			case 300: return 3;
			default: return 0;
			}
        }

        inline AssetModel &OptionModel::asset() const { return *_asset; }

        inline int64_t OptionModel::optionId() const { return _optionId; }
    }
}


#endif //VANTAGEFX_GWTOPTIONMODEL_H
