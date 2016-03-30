//
// Created by AMakarenya on 23.03.2016.
//

#include "SerializableParameter.inl"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, ListParameter &result) {
            return ctx >> checked(result.value);
        }
    }
    namespace serialized {

        std::ostream &ListParameter::write(std::ostream &stream) const {
            return stream << "<list>" << value << "</list>";
        }
    }
}