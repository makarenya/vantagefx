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
            int assetId;
            QString name;
            int moneyBack;
            int rateHi;
            int rateLow;
            double price;

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
            QList<OptionListItem> _items;
        };
    }
}


#endif //VANTAGEFX_OPTIONSLISTMODEL_H
