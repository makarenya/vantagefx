//
// Created by AMakarenya on 23.03.2016.
//

#ifndef QT_SECOND_MAPPARAMETER_H
#define QT_SECOND_MAPPARAMETER_H

#include "SerializableParameter.h"

namespace vantagefx {
    namespace serialized {

        struct MapParameter : SerializableParameter {
            std::map<SerializableParameterPtr, SerializableParameterPtr> value;

            std::ostream &write(std::ostream &stream) const override;
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::MapParameter> {
            static std::string className() { return "net.sf.gilead.pojo.gwt.collection.MapParameter"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::MapParameter &result);
    }
}


#endif //QT_SECOND_MAPPARAMETER_H
