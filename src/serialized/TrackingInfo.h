//
// Created by Alexx on 26.03.16.
//

#ifndef VANTAGEFX_TRACKINGINFO_H
#define VANTAGEFX_TRACKINGINFO_H

#include "SerializableParameter.h"


namespace vantagefx {
    namespace serialized {

        struct TrackingInfo {
            std::string serialuid;
            int some1;
            int64_t some2;
            int64_t some3;
            int some4;
            int some5;
            int some6;
            int some7;
            int some8;
            int some9;
            int64_t some10;
            boost::posix_time::ptime date;
        };
    }
    namespace api {
        template<>
        struct GwtReflectedType<serialized::TrackingInfo>
        {
            static std::string className() { return "com.optionfair.client.model.TrackingInfo"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        GwtParseContext & operator>>(GwtParseContext &ctx, serialized::TrackingInfo &result);
    }
}
namespace std {
    std::ostream & operator<<(std::ostream& stream, const vantagefx::serialized::TrackingInfo &item);
}


#endif //VANTAGEFX_TRACKINGINFO_H
