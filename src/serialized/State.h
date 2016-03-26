//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_STATES_H
#define VANTAGEFX_STATES_H

#include "SerializableParameter.h"


namespace vantagefx {
    namespace serialized {

        struct State {
            std::string serialuid;
            std::string country;
            int order;
            std::string some;
            std::string name;
        };
    }
    namespace api {
        template<>
        struct GwtReflectedType<serialized::State>
        {
            static std::string className() { return "com.optionfair.client.model.State"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext & operator>>(GwtParseContext &ctx, serialized::State &result);
    }
}
namespace std {
    std::ostream & operator<<(std::ostream& stream, const vantagefx::serialized::State &item);
}


#endif //VANTAGEFX_STATES_H
