//
// Created by alexx on 25.03.2016.
//

#include "SubMarket.h"

using namespace vantagefx::serialized;

namespace vantagefx {
	namespace api {

		GwtParseContext& operator>>(GwtParseContext &ctx, SubMarket &result)
		{
			ctx >> result.serialuid;
			ctx >> result.some1;
			ctx >> result.id;
			ctx >> result.some3;
			ctx >> result.some4;
			ctx >> result.name;
			ctx >> result.some5;
			return ctx;
		}
	}
}
namespace std {

	std::ostream& operator<<(std::ostream& stream, const SubMarket &item)
	{
		return stream
			<< "<serialuid>" << item.serialuid << "</serialuid>"
			<< "<some1>" << item.some1 << "</some1>"
			<< "<id>" << item.id << "</id>"
			<< "<some3>" << item.some3 << "</some3>"
			<< "<some4>" << item.some4 << "</some4>"
			<< "<name>" << item.name << "</name>"
			<< "<some5>" << item.some5 << "</some5>"
			;
	}
}