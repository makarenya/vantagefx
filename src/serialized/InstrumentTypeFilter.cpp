//
// Created by alexx on 25.03.2016.
//

#include "InstrumentTypeFilter.h"

using namespace vantagefx::serialized;

namespace vantagefx {
	namespace api {

		GwtParseContext& operator>>(GwtParseContext &ctx, InstrumentTypeFilter &result)
		{
			ctx >> result.serialuid;
			ctx >> result.some1;
			ctx >> result.interval;
			ctx >> result.order;
			ctx >> result.some2;
			ctx >> result.title;
			return ctx;
		}
	}
}
namespace std {

	std::ostream& operator<<(std::ostream& stream, const InstrumentTypeFilter &item)
	{
		return stream
			<< "<serialuid>" << item.serialuid << "</serialuid>"
			<< "<some1>" << item.some1 << "</some1>"
			<< "<interval>" << item.interval << "</interval>"
			<< "<order>" << item.order << "</order>"
			<< "<some2>" << item.some2 << "</some2>"
			<< "<title>" << item.title << "</title>"
			;
	}
}