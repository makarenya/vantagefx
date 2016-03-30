//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_MARKET_H
#define QT_SECOND_MARKET_H

#include "SubMarket.h"
#include "SerializableParameter.inl"


namespace vantagefx {
    namespace serialized {

        struct Market {
            std::string serialuid;
            int some1;
            int some2;
            std::string name;
            std::set<SubMarket> subMarkets;
            int some3;
            std::map<std::string, SerializableParameterPtr> parameters;
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::Market> {
            static std::string className() { return "com.optionfair.client.model.Market"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::Market &result);
    }
}
namespace std {
    ostream &operator<<(ostream &stream, const vantagefx::serialized::Market &item);
}


#endif //QT_SECOND_MARKET_H
