//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_OPTIONSLISTMODEL_H
#define VANTAGEFX_OPTIONSLISTMODEL_H

#include <QtCore>
#include "../model/OptionModel.h"

namespace vantagefx {
    namespace viewmodel {

        struct OptionItem {

            OptionItem(int s = 0);

            int64_t id;
            model::OptionModel::OptionStatus status;
            int bet;
			int seconds;
			bool selected;

			bool toggle();
			QString name() const;
            QColor background() const;
			QColor border() const;
			QColor foreground() const;
        };

		class OptionsListModel : public QAbstractListModel
		{
			Q_OBJECT

		public:

			enum RoleNames
			{
				IdRole = Qt::UserRole + 1,
				NameRole,
				StatusRole,
				BackgroundRole,
				BorderRole,
				ForegroundRole,
				BetRole,
                SelectedRole
			};

			explicit OptionsListModel(QObject *parent);
			int rowCount(const QModelIndex &parent) const override;
			QVariant data(const QModelIndex &index, int role) const override;
            bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
			QHash<int, QByteArray> roleNames() const override;
			void updateOption(const model::OptionModel &item);
			void ids(QSet<int64_t> &list);
			bool remove(QSet<int64_t> &list);
			bool contains(int64_t optionId) const;
			bool isSelected(int64_t optionid) const;

            void setMarketId(int marketId);

            int marketId() const;

			void setAssetId(int assetId);

			int assetId() const;

			void setName(QString name);

			QString name() const;

			void setRateHi(int rateHi);

			int rateHi() const;

			void setRateLow(int rateLow);

			int rateLow() const;

			void setPrice(double price);

			double price() const;

			int64_t order() const;

			void setThreshold(int threshold);

			int threshold() const;

		private:

            int _marketId;
			int _assetId;
			QString _name;
			int _rateHi;
			int _rateLow;
			double _price;
			int _threshold;

			QVector<OptionItem> _options;
		};

        inline int OptionsListModel::marketId() const { return _marketId; }

		inline int OptionsListModel::assetId() const { return _assetId; }

		inline QString OptionsListModel::name() const { return _name; }

		inline int OptionsListModel::rateHi() const { return _rateHi; }

		inline int OptionsListModel::rateLow() const { return _rateLow; }

		inline double OptionsListModel::price() const { return _price; }

		inline int OptionsListModel::threshold() const { return _threshold; }
    }
}


#endif //VANTAGEFX_OPTIONSLISTMODEL_H
