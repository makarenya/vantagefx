//
// Created by alexx on 25.03.2016.
//

#include "InstrumentTypeSuperRel.h"

using namespace vantagefx::serialized;

namespace vantagefx {
	namespace api {

		GwtParseContext& operator>>(GwtParseContext &ctx, InstrumentTypeSuperRel &result)
		{
			ctx >> result.serialuid;
			ctx >> result.some1;
			ctx >> result.id;
			ctx >> result.some2;
			ctx >> result.some3;
			ctx >> result.order;
			return ctx;
		}
	}
}
namespace std {

	std::ostream& operator<<(std::ostream& stream, const InstrumentTypeSuperRel &item)
	{
		return stream
			<< "<serialuid>" << item.serialuid << "</serialuid>"
			<< "<some1>" << item.some1 << "</some1>"
			<< "<id>" << item.id << "</id>"
			<< "<some2>" << item.some2 << "</some2>"
			<< "<some3>" << item.some3 << "</some3>"
			<< "<order>" << item.order << "</order>";
	}
}