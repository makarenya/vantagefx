//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_GWTOPTIONMODEL_H
#define VANTAGEFX_GWTOPTIONMODEL_H

#include <QtCore>


namespace vantagefx {
    namespace model {

        class GwtOptionModel
        {
        public:
            GwtOptionModel() {}

            int optionId() const;

            int assetId() const;

            const QString & name() const;

            int moneyBack() const;

            int seconds() const;

            int rateLow() const;

            double price() const;

            const QDateTime & close() const;

            const QString &market() const;

            const QString &subMarket() const;

            int rate(const QString &name) const;

            void setOptionId(int optionId);

            void setAssetId(int assetId);

            void setName(const QString &name);

            void setMoneyBack(int moneyBack);

            void setSeconds(int seconds);

            void setRateLow(int rateLow);

            void setPrice(double price);

            void setMarket(const QString &market);

            void setSubMarket(const QString &subMarket);

            void setClose(const QDateTime &close);

            void setRate(const QString &name, int value);

        private:
            int _optionId;
            int _assetId;
            QString _name;
            int _moneyBack;
            int _seconds;
            int _rateLow;
            double _price;
            QString _market;
            QString _subMarket;
            QDateTime _close;
            QMap<QString, int> _rates;
        };

        inline int GwtOptionModel::rate(const QString &name) const { return _rates[name]; }

        inline const QDateTime &GwtOptionModel::close() const { return _close; }

        inline const QString &GwtOptionModel::market() const { return _market; }

        inline const QString &GwtOptionModel::subMarket() const { return _subMarket; }

        inline double GwtOptionModel::price() const { return _price; }

        inline int GwtOptionModel::rateLow() const { return _rateLow; }

        inline int GwtOptionModel::seconds() const { return _seconds; }

        inline int GwtOptionModel::moneyBack() const { return _moneyBack; }

        inline const QString &GwtOptionModel::name() const { return _name; }

        inline int GwtOptionModel::assetId() const { return _assetId; }

        inline int GwtOptionModel::optionId() const { return _optionId; }
    }
}


#endif //VANTAGEFX_GWTOPTIONMODEL_H
