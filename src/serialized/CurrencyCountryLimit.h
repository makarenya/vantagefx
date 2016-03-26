//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_CURRENCYCOUNTRYLIMIT_H
#define QT_SECOND_CURRENCYCOUNTRYLIMIT_H

#include <stdint.h>
#include <string>
#include "../api/GwtParseContext.h"


namespace vantagefx {
    namespace serialized {

        struct CurrencyCountryLimit {
            std::string id;
            int some1;
            std::string some2;
            int currencyId;
            int order;
            int some5;
            int64_t some6;
            int64_t some7;
            int64_t some8;
            int64_t some9;
            int64_t some10;
            int64_t some11;
            int64_t some12;
            int64_t some13;
            int64_t some14;
            int64_t some15;
            int64_t some16;
            int64_t some17;
            int64_t some18;
            int64_t some19;
            int64_t some20;
            int64_t some21;
            int64_t some22;
            int64_t some23;
            int64_t some24;
            int64_t some25;
            int64_t some26;
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::CurrencyCountryLimit> {
            static std::string className() { return "com.optionfair.client.model.CurrencyCountryLimit"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::CurrencyCountryLimit &result);
    }
}
namespace std {
    ostream &operator<<(ostream &stream, const vantagefx::serialized::CurrencyCountryLimit &item);
}


#endif //QT_SECOND_CURRENCYCOUNTRYLIMIT_H
