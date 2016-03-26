//
// Created by AMakarenya on 23.03.2016.
//

#include "StringParameter.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, StringParameter &result)
        {
            return ctx >> result.value;
        }
    }
	namespace serialized {
		std::ostream& StringParameter::write(std::ostream& stream) const
		{
			return stream << "<string>" << value << "</string>";
		}
	}
}
