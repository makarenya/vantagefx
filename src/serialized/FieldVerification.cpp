//
// Created by alexx on 26.03.2016.
//

#include "FieldVerification.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, FieldVerification &result) {
            ctx >> result.some1;
            ctx >> result.some2;
            ctx >> result.some3;
            ctx >> result.some4;
            ctx >> result.some5;
            ctx >> result.some6;
            ctx >> result.some7;
            ctx >> result.name;
            ctx >> result.some8;
            ctx >> result.regex;
            ctx >> result.some9;
            ctx >> result.some10;
            return ctx;
        }
    }
}
namespace std {

    std::ostream &operator<<(std::ostream &stream, const FieldVerification &item) {
        return stream
               << "<some1>" << item.some1 << "</some1>"
               << "<some2>" << item.some2 << "</some2>"
               << "<some3>" << item.some3 << "</some3>"
               << "<some4>" << item.some4 << "</some4>"
               << "<some5>" << item.some5 << "</some5>"
               << "<some6>" << item.some6 << "</some6>"
               << "<some7>" << item.some7 << "</some7>"
               << "<name>" << item.name << "</name>"
               << "<some8>" << item.some8 << "</some8>"
               << "<regex>" << item.regex << "</regex>"
               << "<some9>" << item.some9 << "</some9>"
               << "<some10>" << item.some10 << "</some10>";
    }
}