//
// Created by alexx on 25.03.2016.
//

#include "Market.h"

using namespace vantagefx::serialized;

namespace vantagefx {
	namespace api {

		GwtParseContext& operator>>(GwtParseContext &ctx, Market &result)
		{
			ctx >> result.serialuid;
			ctx >> result.some1;
			ctx >> result.some2;
			ctx >> result.name;
			ctx >> checked(result.subMarkets);
			ctx >> result.some3;
			ctx >> checked(result.parameters);
			return ctx;
		}
	}
}
namespace std {

	std::ostream& operator<<(std::ostream& stream, const Market &item)
	{
		return stream
			<< "<serialuid>" << item.serialuid << "</serialuid>"
			<< "<some1>" << item.some1 << "</some1>"
			<< "<some2>" << item.some2 << "</some2>"
			<< "<name>" << item.name << "</name>"
			<< "<subMarkets>" << item.subMarkets << "</subMarkets>"
			<< "<some3>" << item.some3 << "</some3>"
			<< "<parameters>" << item.parameters << "</parameters>"
			;
	}
}