//
// Created by alexx on 25.03.2016.
//

#include "Currency.h"

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::Currency &result) {
            ctx >> result.id;
            ctx >> result.order;
            ctx >> result.name;
            ctx >> result.symbol;
            ctx >> result.some2;
            ctx >> result.some3;
            ctx >> result.abbr;
            ctx >> result.some4;
            ctx >> result.some5;
            return ctx;
        }
    }
}
namespace std {
    ostream &operator<<(ostream &stream, const vantagefx::serialized::Currency &item) {
        return stream
               << "<id>" << item.id << "</id>"
               << "<order>" << item.order << "</order>"
               << "<name>" << item.name << "</name>"
               << "<symbol>" << item.symbol << "</symbol>"
               << "<some2>" << item.some2 << "</some2>"
               << "<some3>" << item.some3 << "</some3>"
               << "<abbr>" << item.abbr << "</abbr>"
               << "<some4>" << item.some4 << "</some4>"
               << "<some5>" << item.some5 << "</some5>";
    }
}


