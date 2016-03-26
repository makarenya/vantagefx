//
// Created by AMakarenya on 22.03.2016.
//

#ifndef QT_SECOND_LUTRESOLVERINITPACKAGE_H
#define QT_SECOND_LUTRESOLVERINITPACKAGE_H

#include "LutType.h"
#include "PaymentMethod.h"
#include "Asset.h"
#include "Country.h"
#include "Currency.h"
#include "CurrencyCountryLimit.h"
#include "InstrumentTypeFilter.h"
#include "InstrumentTypeSuperRel.h"
#include "Market.h"

namespace vantagefx {
    namespace serialized {

        struct LutResolverInitPackage {
            std::map<int,int> someMap1;
            std::vector<LutType> lutTypes;
            std::vector<PaymentMethod> paymentMethods;
			std::vector<Asset> assets;
			std::map<int,int> someMap2;
			std::map<int,int> someMap3;
			std::vector<std::string> blackWhiteList;
			std::vector<int> someList1;
			std::string admin;
			int some1;
			std::string pawwsord;
			std::vector<int> someList2;
			std::vector<Country> counties;
			std::string some2;
			std::vector<Currency> currencies;
			std::vector<CurrencyCountryLimit> limits;
			int some3;
			boost::posix_time::ptime date;
			int some4;
			int some5;
			std::vector<InstrumentTypeFilter> instrumentTypeFilters;
			std::map<int, std::vector<int>> someMap4;
			std::map<int, std::set<int>> someMap5;
			std::vector<InstrumentTypeSuperRel> superRels;
			int some6;
			int some7;
			int some8;
			int some9;
			int some10;
			std::vector<Market> markets;
		};
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::LutResolverInitPackage>
        {
            static std::string className() { return "com.optionfair.client.model.LutResolverInitPackage"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext & operator>>(GwtParseContext &ctx, serialized::LutResolverInitPackage &result);
    }
}
namespace std {
    std::ostream & operator<<(std::ostream& stream, const vantagefx::serialized::LutResolverInitPackage &item);
}


#endif //QT_SECOND_LUTRESOLVERINITPACKAGE_H
