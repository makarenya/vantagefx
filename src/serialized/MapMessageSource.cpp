//
// Created by alexx on 26.03.2016.
//

#include "MapMessageSource.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, MapMessageSource &result) {
            ctx >> checked(result.map);
            ctx >> result.some;
            return ctx;
        }
    }
}
namespace std {

    std::ostream &operator<<(std::ostream &stream, const MapMessageSource &item) {
        return stream
               << "<map>" << item.map << "</map>"
               << "<some>" << item.some << "</some>";

    }
}