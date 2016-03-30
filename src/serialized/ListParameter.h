//
// Created by AMakarenya on 23.03.2016.
//

#ifndef QT_SECOND_LISTPARAMETER_H
#define QT_SECOND_LISTPARAMETER_H


#include "SerializableParameter.h"

namespace vantagefx {
    namespace serialized {

        struct ListParameter : SerializableParameter {
            std::vector<SerializableParameterPtr> value;

            std::ostream &write(std::ostream &stream) const override;
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::ListParameter> {
            static std::string className() { return "net.sf.gilead.pojo.gwt.collection.ListParameter"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::ListParameter &result);
    }
}


#endif //QT_SECOND_LISTPARAMETER_H
