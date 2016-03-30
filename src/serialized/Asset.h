//
// Created by AMakarenya on 23.03.2016.
//

#ifndef QT_SECOND_ASSET_H
#define QT_SECOND_ASSET_H

#include <string>
#include "../api/GwtParseContext.h"

namespace vantagefx {
    namespace serialized {

        struct Asset {
            std::string id;
            // [rO0ABXcEAAAAAA==]
            double value; // 1
            double some1; //0.009
            double some2; //0.04395
            int some3; //1
            std::string some5;
            //121 [Out]
            double some6;
            //1.0005
            std::string name; //536 [Euro-Dollar]
            int some7; //1
            double some8; //15
            std::string fastMarket; //50 [FastMarketDetected]
            int some9; //0
            int order; //4
            double some10; //0
            int some11; // 2
            int some12; //0
            int some13; //19 [class]
            double some14; //2
            int some15; //0
            int some16; //1
            int some17; //0
            int some18; //5
            double some19; //0.03
            int some20; //3
            double some21; //0
            std::string some22; //61 [Boundary]
            int some23; //0
            int some24; //0
            std::string some25; //90 [CancelWithdrawlProfitAdjustment]
            int some26; //3
            int some27; //1
            std::string shortName; //537 [EUR/USD]
            std::string some28; //0
            std::string some29; //0
            std::string some30; //400 [Other]
            double some31; //0.84
        };
    }

    namespace api {
        template<>
        struct GwtReflectedType<serialized::Asset> {
            static std::string className() { return "com.optionfair.client.model.Asset"; }

            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext &operator>>(GwtParseContext &ctx, serialized::Asset &result);
    }
}
namespace std {
    std::ostream &operator<<(std::ostream &stream, const vantagefx::serialized::Asset &item);
}


#endif //QT_SECOND_ASSET_H
