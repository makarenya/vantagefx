//
// Created by alexx on 26.03.2016.
//

#include "State.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, State &result)
        {
            ctx >> result.serialuid;
            ctx >> result.country;
            ctx >> result.order;
            ctx >> result.some;
            ctx >> result.name;
            return ctx;
        }
    }
}
namespace std {

    std::ostream& operator<<(std::ostream& stream, const State &item)
    {
        return stream
               << "<serialuid>" << item.serialuid << "</serialuid>"
               << "<country>" << item.country << "</country>"
               << "<order>" << item.order << "</order>"
               << "<some>" << item.some << "</some>"
               << "<name>" << item.name << "</name>"
                ;

    }
}
