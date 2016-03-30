//
// Created by AMakarenya on 22.03.2016.
//

#ifndef QT_SECOND_LUTTYPE_H
#define QT_SECOND_LUTTYPE_H

#include "Lut.h"
#include "SerializableParameter.h"

namespace vantagefx {
    namespace serialized {

        struct LutType {
            std::string id;
            int order;
            std::string tableName;
            std::vector<Lut> luts;
            int some;
            std::map<std::string, SerializableParameterPtr> parameters;
        };
    }
    namespace api {
        template<>
        struct GwtReflectedType<serialized::LutType> {
            static std::string className() { return "com.optionfair.client.model.LutType"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::LutType &result);
    }
}
namespace std {
    std::ostream &operator<<(std::ostream &stream, const vantagefx::serialized::LutType &item);
}


#endif //QT_SECOND_LUTTYPE_H
