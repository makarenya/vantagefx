//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_OPTIONSLISTMODEL_H
#define VANTAGEFX_OPTIONSLISTMODEL_H

#include <QtCore>
#include "../model/GwtOptionModel.h"

namespace vantagefx {
    namespace viewmodel {

        class OptionsListModel : public QAbstractListModel
        {
            Q_OBJECT

        public:

            enum RoleNames {
                OptionIdRole = Qt::UserRole + 1,
                AssetIdRole,
                NameRole,
                MoneyBackRole,
                RateLowRole,
                RateHiRole,
                PriceRole
            };

	        explicit OptionsListModel(QObject *parent = 0);

			Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

			Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

            QHash<int, QByteArray> roleNames() const override;

        private slots:

            void optionsUpdate(QMap<int, model::GwtOptionModel> options);

        signals:

            void updateOptions(QMap<int, model::GwtOptionModel> options);

        private:
            QList<model::GwtOptionModel> _items;
        };
    }
}


#endif //VANTAGEFX_OPTIONSLISTMODEL_H
