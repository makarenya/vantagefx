//
// Created by AMakarenya on 07.06.2016.
//

#ifndef VANTAGEFX_ASSETLISTMODEL_H
#define VANTAGEFX_ASSETLISTMODEL_H

#include <QtCore>
#include "../model/OptionModel.h"
#include "OptionsListModel.h"

namespace vantagefx {
    namespace viewmodel {

        class AssetListModel : public QAbstractListModel
        {
            Q_OBJECT

        public:

            enum RoleNames {
                AssetIdRole = Qt::UserRole + 1,
                NameRole,
                RateLowRole,
                RateHiRole,
                PriceRole,
				OptionsRole
            };

	        explicit AssetListModel(QObject *parent = nullptr);

			Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

			Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
			Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

			void watchForAsset(const model::AssetModel &asset);
            Q_INVOKABLE void stopWatch(int assetId);

            QHash<int, QByteArray> roleNames() const override;

            void updateAssets(const QMap<int, model::AssetModel> &assets);
            void updateOptions(const QMap<int64_t, model::OptionModel> &options);
			void updateOption(model::OptionModel &item);

            QMap<QString, QList<VirtualBet>> calculateVirtualBets();

			bool containsOption(int64_t optionId) const;
			OptionsListModel &assetFor(int64_t optionId) const;
			bool isSelected(int64_t optionId) const;

        private:
            QList<OptionsListModel *> _items;
        };
    }
}
#endif //VANTAGEFX_ASSETLISTMODEL_H
