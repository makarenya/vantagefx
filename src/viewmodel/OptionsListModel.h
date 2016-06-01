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
				BetRole
			};

			explicit OptionsListModel(QObject *parent);
			int rowCount(const QModelIndex &parent) const override;
			QVariant data(const QModelIndex &index, int role) const override;
			QHash<int, QByteArray> roleNames() const override;
			void updateOption(model::OptionModel &item);
			void ids(QSet<int64_t> &list);
			bool remove(QSet<int64_t> &list);
			bool toggle(int64_t optionId);
			bool contains(int64_t optionId) const;
			bool isSelected(int64_t optionid) const;


			void setAssetId(int assetId);

			int assetId() const;

			void setName(QString name);

			QString name() const;

			void setMoneyBack(int moneyBack);

			int moneyBack() const;

			void setRateHi(int rateHi);

			int rateHi() const;

			void setRateLow(int rateLow);

			int rateLow() const;

			void setPrice(double price);

			double price() const;

			void setMarket(QString market);

			QString market() const;

			void setSubMarket(QString subMarket);

			QString subMarket() const;

			void setLineId(QString lineId);

			QString lineId() const;

			void setThreshold(int threshold);

			int threshold() const;

		private:

			int _assetId;
			QString _name;
			int _moneyBack;
			int _rateHi;
			int _rateLow;
			double _price;
			QString _market;
			QString _subMarket;
			QString _lineId;
			int _threshold;

			QVector<OptionItem> _options;
		};

		inline int OptionsListModel::assetId() const { return _assetId; }

		inline QString OptionsListModel::name() const { return _name; }

		inline int OptionsListModel::moneyBack() const { return _moneyBack; }

		inline int OptionsListModel::rateHi() const { return _rateHi; }

		inline int OptionsListModel::rateLow() const { return _rateLow; }

		inline double OptionsListModel::price() const { return _price; }

		inline QString OptionsListModel::market() const { return _market; }

		inline QString OptionsListModel::subMarket() const { return _subMarket; }

		inline QString OptionsListModel::lineId() const { return _lineId; }

		inline int OptionsListModel::threshold() const { return _threshold; }


        class AssetListModel : public QAbstractListModel
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
				ThresholdRole,
				OptionsRole
            };

	        explicit AssetListModel(QObject *parent = nullptr);

			Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

			Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
			Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

			Q_INVOKABLE void select(long long optionId);

            QHash<int, QByteArray> roleNames() const override;

            void updateOptions(QMap<int64_t, model::OptionModel> &options);
			void updateOption(model::OptionModel &item);

			bool containsOption(int64_t optionId) const;
			OptionsListModel &assetFor(int64_t optionId) const;
			bool isSelected(int64_t optionId) const;

			int threshold(int assetId);

        private:
			static QVector<int> updateOption(OptionsListModel *current, model::OptionModel &item);
			OptionsListModel *createOption(model::OptionModel &item, std::string lineId);
            QList<OptionsListModel *> _items;
        };
    }
}


#endif //VANTAGEFX_OPTIONSLISTMODEL_H
