//
// Created by alexx on 26.03.2016.
//

#include "MTServer.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, MTServer &result) {
            ctx >> result.some1;
            ctx >> result.some2;
            ctx >> result.name;
            return ctx;
        }
    }
}
namespace std {

    std::ostream &operator<<(std::ostream &stream, const MTServer &item) {
        return stream
               << "<some1>" << item.some1 << "</some1>"
               << "<some2>" << item.some2 << "</some2>"
               << "<name>" << item.name << "</name>";

    }
}