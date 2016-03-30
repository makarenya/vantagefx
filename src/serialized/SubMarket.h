//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_SUBMARKET_H
#define QT_SECOND_SUBMARKET_H

#include "../api/GwtParseContext.h"


namespace vantagefx {
    namespace serialized {

        struct SubMarket {
            std::string serialuid;
            int some1;
            int id;
            int some3;
            int some4;
            std::string name;
            int some5;
        };

        inline bool operator<(const SubMarket &x, const SubMarket &y) {
            return x.id < y.id;
        }
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::SubMarket> {
            static std::string className() { return "com.optionfair.client.model.SubMarket"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::SubMarket &result);
    }
}
namespace std {
    ostream &operator<<(ostream &stream, const vantagefx::serialized::SubMarket &item);
}


#endif //QT_SECOND_SUBMARKET_H
