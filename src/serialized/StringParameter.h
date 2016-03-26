//
// Created by AMakarenya on 23.03.2016.
//

#ifndef QT_SECOND_STRINGPARAMETER_H
#define QT_SECOND_STRINGPARAMETER_H


#include "SerializableParameter.h"

namespace vantagefx {
    namespace serialized {

        struct StringParameter : SerializableParameter {
            std::string value;
			std::ostream &write(std::ostream &stream) const override;
		};
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::StringParameter>
        {
            static std::string className() { return "net.sf.gilead.pojo.gwt.basic.StringParameter"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext & operator>>(GwtParseContext &ctx, serialized::StringParameter &result);
    }
}

#endif //QT_SECOND_STRINGPARAMETER_H
