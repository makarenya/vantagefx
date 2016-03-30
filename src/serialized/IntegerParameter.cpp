//
// Created by AMakarenya on 22.03.2016.
//

#include "IntegerParameter.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, IntegerParameter &result) {
            return ctx >> checked(result.value);
        }
    }
    namespace serialized {
        std::ostream &IntegerParameter::write(std::ostream &stream) const {
            return stream << "<integer>" << value << "</integer>";
        }
    }
}