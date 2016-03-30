//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_PSPCCTYPEREL_H
#define VANTAGEFX_PSPCCTYPEREL_H

#include "SerializableParameter.h"

namespace vantagefx {
    namespace serialized {

        struct PspCcTypeRel {
            std::string serialuid;
            int some1;
            int64_t some2;
            int some3;
        };
    }
    namespace api {
        template<>
        struct GwtReflectedType<serialized::PspCcTypeRel> {
            static std::string className() { return "com.optionfair.client.model.PspCcTypeRel"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::PspCcTypeRel &result);
    }
}
namespace std {
    std::ostream &operator<<(std::ostream &stream, const vantagefx::serialized::PspCcTypeRel &item);
}


#endif //VANTAGEFX_PSPCCTYPEREL_H
