//
// Created by Alexx on 26.03.16.
//

#include "TrackingInfo.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, TrackingInfo &result) {
            ctx >> result.serialuid;
            ctx >> result.some1;
            ctx >> result.some2;
            ctx >> result.some3;
            ctx >> result.some4;
            ctx >> result.some5;
            ctx >> result.some6;
            ctx >> result.some7;
            ctx >> result.some8;
            ctx >> result.some9;
            ctx >> result.some10;
            ctx >> checked(result.date);
            return ctx;
        }
    }
}
namespace std {

    std::ostream &operator<<(std::ostream &stream, const TrackingInfo &item) {
        return stream
               << "<serialuid>" << item.serialuid << "</serialuid>"
               << "<some1>" << item.some1 << "</some1>"
               << "<some2>" << item.some2 << "</some2>"
               << "<some3>" << item.some3 << "</some3>"
               << "<some4>" << item.some4 << "</some4>"
               << "<some5>" << item.some5 << "</some5>"
               << "<some6>" << item.some6 << "</some6>"
               << "<some7>" << item.some7 << "</some7>"
               << "<some8>" << item.some8 << "</some8>"
               << "<some9>" << item.some9 << "</some9>"
               << "<some10>" << item.some10 << "</some10>"
               << "<date>" << item.date << "</date>";

    }
}
