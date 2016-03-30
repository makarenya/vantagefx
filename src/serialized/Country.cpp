//
// Created by alexx on 25.03.2016.
//

#include "Country.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, Country &result) {
            ctx >> result.id;
            ctx >> result.some1;
            ctx >> result.language;
            ctx >> result.some2;
            ctx >> result.some3;
            ctx >> result.short2;
            ctx >> result.short3;
            ctx >> result.fullName;
            ctx >> result.some4;
            ctx >> result.someNumber;
            ctx >> result.fullName2;
            return ctx;
        }
    }
}

namespace std {
    ostream &operator<<(ostream &stream, const Country &item) {
        return stream
               << "<id>" << item.id << "</id>"
               << "<some1>" << item.some1 << "</some1>"
               << "<language>" << item.language << "</language>"
               << "<some2>" << item.some2 << "</some2>"
               << "<some3>" << item.some3 << "</some3>"
               << "<short2>" << item.short2 << "</short2>"
               << "<short3>" << item.short3 << "</short3>"
               << "<fullName>" << item.fullName << "</fullName>"
               << "<some4>" << item.some4 << "</some4>"
               << "<someNumber>" << item.someNumber << "</someNumber>"
               << "<fullName2>" << item.fullName2 << "</fullName2>";
    }
}
