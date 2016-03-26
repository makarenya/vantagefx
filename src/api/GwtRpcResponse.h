//
// Created by alexx on 21.03.2016.
//

#ifndef QT_SECOND_GWTRPCRESPONSE_H
#define QT_SECOND_GWTRPCRESPONSE_H

#include <boost/variant.hpp>
#include <string>
#include <vector>
#include "GwtResponseData.h"

namespace vantagefx {
    namespace api {

        class GwtRpcResponse
        {
        public:
            GwtRpcResponse() {}
            GwtRpcResponse(int version, int flags, JsonVariantList data, StringList stringTable);

            int version() const { return _version; }
            GwtFlags flags() const { return _flags; }
            std::string url() const { return _url; }
            std::string strongName() const { return _strongName; }
            std::string serviceName() const { return _serviceName; }
            std::string methodName() const { return _methodName; }

        private:
            int _version;
            GwtFlags _flags;
            std::string _url;
            std::string _strongName;
            std::string _serviceName;
            std::string _methodName;
        };
    }
}

#endif //QT_SECOND_GWTRPCRESPONSE_H
