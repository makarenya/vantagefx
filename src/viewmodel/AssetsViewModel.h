//
// Created by AMakarenya on 01.06.2016.
//

#ifndef VANTAGEFX_ASSETSVIEWMODEL_H
#define VANTAGEFX_ASSETSVIEWMODEL_H

#include <QtCore>
#include "../model/AssetModel.h"


namespace vantagefx {
    namespace viewmodel {

        class AssetsViewModel : public QAbstractListModel
        {
			Q_OBJECT
        public:

            enum RoleNames {
                IdRole = Qt::UserRole + 1,
                NameRole
            };

            AssetsViewModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

            Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

            Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

            QHash<int, QByteArray> roleNames() const override;

			void setList(const QMap<int, model::AssetModel> &list);

            int assetId(int index) const;

        private:

            QVector<QPair<int, QString>> _assets;
        };
    }
}

#endif //VANTAGEFX_ASSETSVIEWMODEL_H
