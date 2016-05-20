//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_OPTIONSLISTMODEL_H
#define VANTAGEFX_OPTIONSLISTMODEL_H

#include <QtCore>
#include <QColor>
#include "../model/OptionModel.h"

namespace vantagefx {
    namespace viewmodel {

        struct OptionItem {

            OptionItem();

            int64_t id;
            model::OptionModel::OptionStatus status;
            int bet;

            QColor color() const;
        };


        struct AssetListItem {

			AssetListItem();

            int assetId;
            QString name;
            int moneyBack;
            int rateHi;
            int rateLow;
            double price;
			QString market;
			QString subMarket;
			QString lineId;
			int threshold;

            QVector<OptionItem> options;
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

            void updateOptions(QMap<int64_t, model::OptionModel> &options);
			void updateOption(model::OptionModel &item);

			int threshold(int assetId);

        private:
			static QVector<int> updateOption(AssetListItem &current, model::OptionModel &item);
			static AssetListItem createOption(model::OptionModel &item, std::string lineId);
            QList<AssetListItem> _items;
			QMap<int64_t, model::OptionModel> *_options;
        };
    }
}


#endif //VANTAGEFX_OPTIONSLISTMODEL_H
