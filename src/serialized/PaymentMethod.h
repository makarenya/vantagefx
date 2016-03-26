//
// Created by AMakarenya on 23.03.2016.
//

#ifndef QT_SECOND_PAYMENTMETHOD_H
#define QT_SECOND_PAYMENTMETHOD_H

#include <string>
#include "../api/GwtParseContext.h"

namespace vantagefx {
    namespace serialized {

        struct PaymentMethod {
            std::string id;
            int someId;
            double someValue;
            int taxName;
            int some1;
            int some2;
            int some3;
            std::string name;
            int some4;
            int some5;
            int some6;
            int otherValue;
			int some7;
            std::string otherName;
        };
    }

    namespace api {
		template<>
		struct GwtReflectedType<serialized::PaymentMethod>
		{
			static std::string className() { return "com.optionfair.client.model.PaymentMethod"; }
			static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
		};

		GwtParseContext & operator>>(GwtParseContext &ctx, serialized::PaymentMethod &result);
    }
}
namespace std {
    std::ostream & operator<<(std::ostream& stream, const vantagefx::serialized::PaymentMethod &item);
}


#endif //QT_SECOND_PAYMENTMETHOD_H
