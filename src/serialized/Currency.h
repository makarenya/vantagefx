//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_CURRENCY_H
#define QT_SECOND_CURRENCY_H

#include <stdint.h>
#include <string>
#include "../api/GwtParseContext.h"

namespace vantagefx {
    namespace serialized {

        struct Currency {
            std::string id;
            int order;
            std::string name;
            std::string symbol;
            double some2;
            int some3;
            std::string abbr;
            int some4;
            int64_t some5;
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::Currency> {
            static std::string className() { return "com.optionfair.client.model.Currency"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::Currency &result);
    }
}
namespace std {
    ostream &operator<<(ostream &stream, const vantagefx::serialized::Currency &item);
}


#endif //QT_SECOND_CURRENCY_H
