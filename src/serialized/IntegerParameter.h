//
// Created by AMakarenya on 22.03.2016.
//

#ifndef QT_SECOND_INTEGERPARAMETER_H
#define QT_SECOND_INTEGERPARAMETER_H

#include "SerializableParameter.h"

namespace vantagefx {
    namespace serialized {

        struct IntegerParameter : SerializableParameter {
            int value;
			std::ostream &write(std::ostream &stream) const override;
		};
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::IntegerParameter>
        {
            static std::string className() { return "net.sf.gilead.pojo.gwt.basic.IntegerParameter"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext & operator>>(GwtParseContext &ctx, serialized::IntegerParameter &result);
    }
}


#endif //QT_SECOND_INTEGERPARAMETER_H
