//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_OPTIONSLISTMODEL_H
#define VANTAGEFX_OPTIONSLISTMODEL_H

#include <QtCore>
#include "../model/GwtOptionModel.h"

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
				  option300(0)
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

            int option30;
            int option60;
            int option120;
            int option300;
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
                Option300Role
            };

	        explicit OptionsListModel(QObject *parent = 0);

			Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

			Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

            QHash<int, QByteArray> roleNames() const override;

            void updateOptions(std::map<int, model::GwtOptionModel> options);

        private:
			static QVector<int> updateOption(OptionListItem &current, model::GwtOptionModel &item, int hi, int lo);
			static OptionListItem createOption(model::GwtOptionModel &item, std::string lineId, int hi, int lo);
            QList<OptionListItem> _items;
        };
    }
}


#endif //VANTAGEFX_OPTIONSLISTMODEL_H
