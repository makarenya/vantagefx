//
// Created by AMakarenya on 22.03.2016.
//

#include "LutType.h"
#include "SerializableParameter.inl"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, LutType &result)
        {
            return ctx >> result.id >> result.order >> result.tableName >> checked(result.luts)
                   >> result.some >> checked(result.parameters);
        }
    }
}
namespace std {

    std::ostream& operator<<(std::ostream& stream, const LutType &item)
    {
		return stream
			<< "<id>" << item.id << "</id>"
			<< "<order>" << item.order << "</order>"
			<< "<tableName>" << item.tableName << "</tableName>"
			<< "<luts>" << item.luts << "</luts>"
			<< "<some>" << item.some << "</some>"
			<< "<parameters>" << item.parameters << "</parameters>"
			;

    }
}