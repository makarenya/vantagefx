//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_MAPMESSAGESOURCE_H
#define VANTAGEFX_MAPMESSAGESOURCE_H

#include "../api/GwtParseContext.h"

namespace vantagefx {
    namespace serialized {

        struct MapMessageSource {
            std::map<std::string, std::string> map;
            int some;
        };
    }
    namespace api {
        template<>
        struct GwtReflectedType<serialized::MapMessageSource> {
            static std::string className() { return "com.optionfair.client.messages.MapMessageSource"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::MapMessageSource &result);
    }
}
namespace std {
    std::ostream &operator<<(std::ostream &stream, const vantagefx::serialized::MapMessageSource &item);
}


#endif //VANTAGEFX_MAPMESSAGESOURCE_H
