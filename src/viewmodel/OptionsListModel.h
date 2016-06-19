//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_OPTIONSLISTMODEL_H
#define VANTAGEFX_OPTIONSLISTMODEL_H

#include <QtCore>
#include "OptionItem.h"
#include "../model/OptionModel.h"

namespace vantagefx {
    namespace viewmodel {

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

			const TimePoint *timePoint(QDateTime maxTime);
			int dynamic(const TimePoint *end, int interval);

            QList<VirtualBet> calculateVirtualBets();

            void setMarketId(int marketId);

            int marketId() const;

			void setAssetId(int assetId);

			int assetId() const;

			void setName(QString name);

			QString name() const;

			void setRateHi(int rateHi);

			void setAllRates(const QMap<QString, int> &rates);

			int rateHi() const;

			void setRateLow(int rateLow);

			int rateLow() const;

			void setPrice(double price);

			double price() const;

			int64_t order() const;

		private:

            int _marketId;
			int _assetId;
			QString _name;
			int _rateHi;
			int _rateLow;
			double _price;

			QVector<OptionItem> _options;
            QLinkedList<TimePoint> _sequence;
			QMap<QString, int> _rates;
		};

        inline int OptionsListModel::marketId() const { return _marketId; }

		inline int OptionsListModel::assetId() const { return _assetId; }

		inline QString OptionsListModel::name() const { return _name; }

		inline int OptionsListModel::rateHi() const { return _rateHi; }

		inline int OptionsListModel::rateLow() const { return _rateLow; }

		inline double OptionsListModel::price() const { return _price; }
    }
}


#endif //VANTAGEFX_OPTIONSLISTMODEL_H
