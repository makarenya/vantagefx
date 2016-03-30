//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_INSTRUMENTTYPEINSTRUMENTTYPESUPERREL_H
#define QT_SECOND_INSTRUMENTTYPEINSTRUMENTTYPESUPERREL_H


#include "../api/GwtParseContext.h"

namespace vantagefx {
    namespace serialized {

        struct InstrumentTypeSuperRel {
            std::string serialuid;
            int some1;
            int id;
            int some2;
            int some3;
            int order;
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::InstrumentTypeSuperRel> {
            static std::string className() { return "com.optionfair.client.model.InstrumentTypeInstrumentTypeSuperRel"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::InstrumentTypeSuperRel &result);
    }
}
namespace std {
    ostream &operator<<(ostream &stream, const vantagefx::serialized::InstrumentTypeSuperRel &item);
}


#endif //QT_SECOND_INSTRUMENTTYPEINSTRUMENTTYPESUPERREL_H
