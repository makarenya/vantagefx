//
// Created by alexx on 26.03.2016.
//

#include "PspCcTypeRel.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, PspCcTypeRel &result)
        {
            ctx >> result.serialuid;
            ctx >> result.some1;
            ctx >> result.some2;
            ctx >> result.some3;
            return ctx;
        }
    }
}
namespace std {

    std::ostream& operator<<(std::ostream& stream, const PspCcTypeRel &item)
    {
        return stream
               << "<serialuid>" << item.serialuid << "</serialuid>"
               << "<some1>" << item.some1 << "</some1>"
               << "<some2>" << item.some2 << "</some2>"
               << "<some3>" << item.some3 << "</some3>"
                ;

    }
}
