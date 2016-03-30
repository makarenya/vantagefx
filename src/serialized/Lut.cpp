//
// Created by AMakarenya on 22.03.2016.
//

#include "Lut.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, Lut &result) {
            return ctx >> result.parent >> result.order >> result.word >> result.some;
        }
    }
}
namespace std {

    std::ostream &operator<<(std::ostream &stream, const Lut &item) {
        return stream
               << "<parent>" << item.parent << "</parent>"
               << "<order>" << item.order << "</order>"
               << "<word>" << item.word << "</word>"
               << "<some>" << item.some << "</some>";
    }
}

