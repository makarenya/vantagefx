//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_OPTIONSLISTMODEL_H
#define VANTAGEFX_OPTIONSLISTMODEL_H

#include <QtCore>
#include "../model/OptionModel.h"

namespace vantagefx {
    namespace viewmodel {

        struct OptionListItem {

			OptionListItem() 
				: assetId(0),
				  moneyBack(0),
				  rateHi(0),
				  rateLow(0),
				  price(0.0),
				  option30(0),
				  option60(0),
				  option120(0),
				  option300(0),
				  threshold(71)
			{}

            int assetId;
            QString name;
            int moneyBack;
            int rateHi;
            int rateLow;
            double price;
			QString market;
			QString subMarket;
			QString lineId;

            int64_t option30;
			int64_t option60;
			int64_t option120;
			int64_t option300;

			int threshold;
			QString color30;
			QString color60;
			QString color120;
			QString color300;
		};

        class OptionsListModel : public QAbstractListModel
        {
            Q_OBJECT

        public:

            enum RoleNames {
                AssetIdRole = Qt::UserRole + 1,
                NameRole,
                MoneyBackRole,
                RateLowRole,
                RateHiRole,
                PriceRole,
                Option30Role,
                Option60Role,
                Option120Role,
                Option300Role,
				ThresholdRole,
				Color30Role,
				Color60Role,
				Color120Role,
				Color300Role
            };

	        explicit OptionsListModel(QObject *parent = nullptr);

			Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

			Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
			Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

			Q_INVOKABLE void select(long long optionId);

            QHash<int, QByteArray> roleNames() const override;

            void updateOptions(const QMap<int64_t, model::OptionModel> &options);

			int threshold(int assetId);

        private:
	        static QString color(model::OptionModel &option);
			static QVector<int> updateOption(OptionListItem &current, model::OptionModel &item);
			static OptionListItem createOption(model::OptionModel &item, std::string lineId);
            QList<OptionListItem> _items;
        };
    }
}


#endif //VANTAGEFX_OPTIONSLISTMODEL_H
