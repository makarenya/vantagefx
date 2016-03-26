//
// Created by AMakarenya on 22.03.2016.
//

#include "LutResolverInitPackage.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, LutResolverInitPackage &result)
        {
			ctx >> checked(result.someMap1);
			ctx >> checked(result.lutTypes);
			ctx >> checked(result.paymentMethods);
			ctx >> checked(result.assets);
			ctx >> checked(result.someMap2);
			ctx >> checked(result.someMap3);
			ctx >> checked(result.blackWhiteList);
			ctx >> checked(result.someList1);
			ctx >> result.admin;
			ctx >> result.some1;
			ctx >> result.pawwsord;
			ctx >> checked(result.someList2);
			ctx >> checked(result.counties);
			ctx >> result.some2;
			ctx >> checked(result.currencies);
			ctx >> checked(result.limits);
			ctx >> result.some3;
			ctx >> checked(result.date);
			ctx >> result.some4;
			ctx >> result.some5;
			ctx >> checked(result.instrumentTypeFilters);
			ctx >> checked(result.someMap4);
			ctx >> checked(result.someMap5);
			ctx >> checked(result.superRels);
			ctx >> result.some6;
			ctx >> result.some7;
			ctx >> result.some8;
			ctx >> result.some9;
			ctx >> result.some10;
			ctx >> checked(result.markets);
			return ctx;
        }
    }
}
namespace std {

    std::ostream& operator<<(std::ostream &stream, const LutResolverInitPackage &item)
    {
		return stream
			<< "<someMap1>" << item.someMap1 << "</someMap1>"
			<< "<lutTypes>" << item.lutTypes << "</lutTypes>"
			<< "<paymentMethods>" << item.paymentMethods << "</paymentMethods>"
			<< "<assets>" << item.assets << "</assets>"
			<< "<someMap2>" << item.someMap2 << "</someMap2>"
			<< "<someMap3>" << item.someMap3 << "</someMap3>"
			<< "<blackWhiteList>" << item.blackWhiteList << "</blackWhiteList>"
			<< "<someList1>" << item.someList1 << "</someList1>"
			<< "<admin>" << item.admin << "</admin>"
			<< "<some1>" << item.some1 << "</some1>"
			<< "<pawwsord>" << item.pawwsord << "</pawwsord>"
			<< "<someList2>" << item.someList2 << "</someList2>"
			<< "<counties>" << item.counties << "</counties>"
			<< "<some2>" << item.some2 << "</some2>"
			<< "<currencies>" << item.currencies << "</currencies>"
			<< "<limits>" << item.limits << "</limits>"
			<< "<some3>" << item.some3 << "</some3>"
			<< "<date>" << item.date << "</date>"
			<< "<some4>" << item.some4 << "</some4>"
			<< "<some5>" << item.some5 << "</some5>"
			<< "<instrumentTypeFilters>" << item.instrumentTypeFilters << "</instrumentTypeFilters>"
			<< "<someMap4>" << item.someMap4 << "</someMap4>"
			<< "<someMap5>" << item.someMap5 << "</someMap5>"
			<< "<superRels>" << item.superRels << "</superRels>"
			<< "<some6>" << item.some6 << "</some6>"
			<< "<some7>" << item.some7 << "</some7>"
			<< "<some8>" << item.some8 << "</some8>"
			<< "<some9>" << item.some9 << "</some9>"
			<< "<some10>" << item.some10 << "</some10>"
			<< "<markets>" << item.markets << "</markets>"
			;
	}
}

