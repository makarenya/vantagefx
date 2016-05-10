//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_GWTOPTIONMODEL_H
#define VANTAGEFX_GWTOPTIONMODEL_H

#include <QtCore>
#include <map>
#include <string>


namespace vantagefx {
    namespace model {

        class GwtOptionModel
        {
        public:

			GwtOptionModel();

			int64_t optionId() const;

            int assetId() const;

            const std::string &name() const;

            int moneyBack() const;

            int seconds() const;

			int returnValue() const;

            int rateLow() const;

            double price() const;

            int64_t close() const;

			int marketId() const;

            const std::string &market() const;

            const std::string &subMarket() const;

            int rate(const std::string &name) const;

            void setOptionId(int64_t optionId);

            void setAssetId(int assetId);

            void setName(const std::string &name);

            void setMoneyBack(int moneyBack);

            void setSeconds(int seconds);

			void setReturnValue(int returnValue);

            void setRateLow(int rateLow);

            void setPrice(double price);

			void setMarketId(int marketId);

            void setMarket(const std::string &market);

            void setSubMarket(const std::string &subMarket);

            void setClose(int64_t close);

            void setRate(const std::string &name, int value);

        private:
            int64_t _optionId;
            int _assetId;
            std::string _name;
            int _moneyBack;
            int _seconds;
			int _returnValue;
            int _rateLow;
            double _price;
			int _marketId;
			std::string _market;
			std::string _subMarket;
            int64_t _close;
            std::map<std::string, int> _rates;
        };

        inline int64_t GwtOptionModel::close() const { return _close; }

		inline int GwtOptionModel::marketId() const { return _marketId; }

        inline const std::string &GwtOptionModel::market() const { return _market; }

        inline const std::string &GwtOptionModel::subMarket() const { return _subMarket; }

        inline double GwtOptionModel::price() const { return _price; }

        inline int GwtOptionModel::rateLow() const { return _rateLow; }

        inline int GwtOptionModel::seconds() const { return _seconds; }

		inline int GwtOptionModel::returnValue() const { return _returnValue; }

        inline int GwtOptionModel::moneyBack() const { return _moneyBack; }

        inline const std::string &GwtOptionModel::name() const { return _name; }

        inline int GwtOptionModel::assetId() const { return _assetId; }

        inline int64_t GwtOptionModel::optionId() const { return _optionId; }
    }
}


#endif //VANTAGEFX_GWTOPTIONMODEL_H
