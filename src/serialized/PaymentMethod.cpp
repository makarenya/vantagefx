//
// Created by AMakarenya on 23.03.2016.
//

#include "PaymentMethod.h"

using namespace vantagefx::serialized;

namespace vantagefx {
    namespace api {

        GwtParseContext &operator>>(GwtParseContext &ctx, PaymentMethod &result) {
            ctx >> result.id;
            ctx >> result.someId;
            ctx >> result.someValue;
            ctx >> result.taxName;
            ctx >> result.some1;
            ctx >> result.some2;
            ctx >> result.some3;
            ctx >> result.name;
            ctx >> result.some4;
            ctx >> result.some5;
            ctx >> result.some6;
            ctx >> result.otherValue;
            ctx >> result.some7;
            ctx >> result.otherName;
            return ctx;
        }
    }
}
namespace std {

    std::ostream &operator<<(std::ostream &stream, const PaymentMethod &item) {
        return stream
               << "<id>" << item.id << "</id>"
               << "<someId>" << item.someId << "</someId>"
               << "<someValue>" << item.someValue << "</someValue>"
               << "<taxName>" << item.taxName << "</taxName>"
               << "<some1>" << item.some1 << "</some1>"
               << "<some2>" << item.some2 << "</some2>"
               << "<some3>" << item.some3 << "</some3>"
               << "<name>" << item.name << "</name>"
               << "<some4>" << item.some4 << "</some4>"
               << "<some5>" << item.some5 << "</some5>"
               << "<some6>" << item.some6 << "</some6>"
               << "<otherValue>" << item.otherValue << "</otherValue>"
               << "<some7>" << item.some7 << "</some7>"
               << "<otherName>" << item.otherName << "</otherName>";
    }
}

