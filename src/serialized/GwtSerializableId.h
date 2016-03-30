//
// Created by AMakarenya on 22.03.2016.
//

#ifndef QT_SECOND_GWTSERIALIZABLEID_H
#define QT_SECOND_GWTSERIALIZABLEID_H

#include <string>
#include <src/api/GwtParseContext.h>
#include "SerializableParameter.h"


namespace vantagefx {
    namespace serialized {

        struct GwtSerializableId : SerializableParameter {
            SerializableParameterPtr id;
            std::string entityName;
            std::string stringValue;

            std::ostream &write(std::ostream &stream) const override;
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::GwtSerializableId> {
            static std::string className() { return "net.sf.gilead.pojo.gwt.GwtSerializableId"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::GwtSerializableId &result);
    }
}

#endif //QT_SECOND_GWTSERIALIZABLEID_H
