//
// Created by alexx on 25.03.2016.
//

#include "CurrencyCountryLimit.h"

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::CurrencyCountryLimit &result) {
            ctx >> result.id;
            ctx >> result.some1;
            ctx >> result.some2;
            ctx >> result.currencyId;
            ctx >> result.order;
            ctx >> result.some5;
            ctx >> result.some6;
            ctx >> result.some7;
            ctx >> result.some8;
            ctx >> result.some9;
            ctx >> result.some10;
            ctx >> result.some11;
            ctx >> result.some12;
            ctx >> result.some13;
            ctx >> result.some14;
            ctx >> result.some15;
            ctx >> result.some16;
            ctx >> result.some17;
            ctx >> result.some18;
            ctx >> result.some19;
            ctx >> result.some20;
            ctx >> result.some21;
            ctx >> result.some22;
            ctx >> result.some23;
            ctx >> result.some24;
            ctx >> result.some25;
            ctx >> result.some26;
            return ctx;
        }
    }
}
namespace std {
    ostream &operator<<(ostream &stream, const vantagefx::serialized::CurrencyCountryLimit &item) {
        return stream
               << "<id>" << item.id << "</id>"
               << "<some1>" << item.some1 << "</some1>"
               << "<some2>" << item.some2 << "</some2>"
               << "<currencyId>" << item.currencyId << "</currencyId>"
               << "<order>" << item.order << "</order>"
               << "<some5>" << item.some5 << "</some5>"
               << "<some6>" << item.some6 << "</some6>"
               << "<some7>" << item.some7 << "</some7>"
               << "<some8>" << item.some8 << "</some8>"
               << "<some9>" << item.some9 << "</some9>"
               << "<some10>" << item.some10 << "</some10>"
               << "<some11>" << item.some11 << "</some11>"
               << "<some12>" << item.some12 << "</some12>"
               << "<some13>" << item.some13 << "</some13>"
               << "<some14>" << item.some14 << "</some14>"
               << "<some15>" << item.some15 << "</some15>"
               << "<some16>" << item.some16 << "</some16>"
               << "<some17>" << item.some17 << "</some17>"
               << "<some18>" << item.some18 << "</some18>"
               << "<some19>" << item.some19 << "</some19>"
               << "<some20>" << item.some20 << "</some20>"
               << "<some21>" << item.some21 << "</some21>"
               << "<some22>" << item.some22 << "</some22>"
               << "<some23>" << item.some23 << "</some23>"
               << "<some24>" << item.some24 << "</some24>"
               << "<some25>" << item.some25 << "</some25>"
               << "<some26>" << item.some26 << "</some26>";
    }
}


