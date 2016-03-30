//
// Created by AMakarenya on 22.03.2016.
//

#include "SerializableParameter.inl"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        api::GwtParseContext &operator>>(GwtParseContext &ctx, GwtSerializableId &result) {
            return ctx >> result.entityName >> checked(result.id) >> result.stringValue;
        }
    }
    namespace serialized {

        std::ostream &GwtSerializableId::write(std::ostream &stream) const {
            return stream
                   << "<id>" << id << "</id>"
                   << "<entityName>" << entityName << "</entityName>"
                   << "<stringValue>" << stringValue << "</stringValue>";
        }
    }
}
