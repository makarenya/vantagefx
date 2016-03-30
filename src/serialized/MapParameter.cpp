//
// Created by AMakarenya on 23.03.2016.
//

#include "SerializableParameter.inl"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, MapParameter &result) {
            return ctx >> checked(result.value);
        }
    }
    namespace serialized {

        std::ostream &MapParameter::write(std::ostream &stream) const {
            return stream << "<map>" << value << "</map>";
        }
    }
}
