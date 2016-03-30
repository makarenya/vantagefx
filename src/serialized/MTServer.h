//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_MTSERVER_H
#define VANTAGEFX_MTSERVER_H

#include "SerializableParameter.h"


namespace vantagefx {
    namespace serialized {

        struct MTServer {
            int some1;
            int some2;
            std::string name;
        };
    }
    namespace api {
        template<>
        struct GwtReflectedType<serialized::MTServer> {
            static std::string className() { return "com.optionfair.client.model.MTServer"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::MTServer &result);
    }
}
namespace std {
    std::ostream &operator<<(std::ostream &stream, const vantagefx::serialized::MTServer &item);
}

#endif //VANTAGEFX_MTSERVER_H
