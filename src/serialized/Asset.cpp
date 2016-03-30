//
// Created by AMakarenya on 23.03.2016.
//

#include "Asset.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, Asset &result) {
            ctx >> result.id;
            ctx >> result.value;
            ctx >> result.some1;
            ctx >> result.some2;
            ctx >> result.some3;
            ctx >> result.some5;
            ctx >> result.some6;
            ctx >> result.name;
            ctx >> result.some7;
            ctx >> result.some8;
            ctx >> result.fastMarket;
            ctx >> result.some9;
            ctx >> result.order;
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
            ctx >> result.some27;
            ctx >> result.shortName;
            ctx >> result.some28;
            ctx >> result.some29;
            ctx >> result.some30;
            ctx >> result.some31;;
            return ctx;
        }
    }
}
namespace std {

    std::ostream &operator<<(std::ostream &stream, const Asset &item) {
        return stream
               << "<id>" << item.id << "</id>"
               << "<value>" << item.value << "</value>"
               << "<some1>" << item.some1 << "</some1>"
               << "<some2>" << item.some2 << "</some2>"
               << "<some3>" << item.some3 << "</some3>"
               << "<some5>" << item.some5 << "</some5>"
               << "<some6>" << item.some6 << "</some6>"
               << "<name>" << item.name << "</name>"
               << "<some7>" << item.some7 << "</some7>"
               << "<some8>" << item.some8 << "</some8>"
               << "<fastMarket>" << item.fastMarket << "</fastMarket>"
               << "<some9>" << item.some9 << "</some9>"
               << "<order>" << item.order << "</order>"
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
               << "<some26>" << item.some26 << "</some26>"
               << "<some27>" << item.some27 << "</some27>"
               << "<shortName>" << item.shortName << "</shortName>"
               << "<some28>" << item.some28 << "</some28>"
               << "<some29>" << item.some29 << "</some29>"
               << "<some30>" << item.some30 << "</some30>"
               << "<some31>" << item.some31 << "</some31>";
    }
}

