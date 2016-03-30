//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_FIELDVERIFICATION_H
#define VANTAGEFX_FIELDVERIFICATION_H

#include <src/api/GwtParseContext.h>


namespace vantagefx {
    namespace serialized {

        struct FieldVerification {
            int some1;
            int some2;
            int some3;
            int some4;
            int some5;
            int some6;
            int some7;
            std::string name;
            int some8;
            std::string regex;
            int some9;
            int some10;
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::FieldVerification> {
            static std::string className() { return "com.optionfair.client.verification.FieldVerification"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::FieldVerification &result);
    }
}
namespace std {
    ostream &operator<<(ostream &stream, const vantagefx::serialized::FieldVerification &item);
}


#endif //VANTAGEFX_FIELDVERIFICATION_H
