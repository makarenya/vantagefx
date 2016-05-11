//
// Created by AMakarenya on 11.05.2016.
//

#ifndef VANTAGEFX_ASSETMODEL_H
#define VANTAGEFX_ASSETMODEL_H

#include <QtCore>

namespace vantagefx {
    namespace model {

        class AssetModel {
        public:
            AssetModel();

	        void setId(int id);
            int id() const;

	        void setName(const QString &name);
            const QString & name() const;

	        void setPrice(double price);
            double price() const;

	        void setMarketId(int marketId);
            int marketId() const;

	        void setMarketName(const QString &marketName);
            const QString &marketName() const;

	        void setSubMarketName(const QString &subMarketName);
            const QString &subMarketName() const;

	        void setRate(const QString &name, int value);
	        int rate(const QString& name) const;

        private:
            int _id;
            QString _name;
            double _price;
            int _marketId;
            QString _marketName;
            QString _subMarketName;
			QMap<QString, int> _rates;
        };


        inline int AssetModel::id() const { return _id; }

        inline const QString &AssetModel::name() const { return _name; }

        inline double AssetModel::price() const { return _price; }

        inline int AssetModel::marketId() const { return _marketId; }

        inline const QString &AssetModel::marketName() const { return _marketName; }

        inline const QString &AssetModel::subMarketName() const { return _subMarketName; }
    }
}


#endif //VANTAGEFX_ASSETMODEL_H
