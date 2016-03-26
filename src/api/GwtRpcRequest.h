//
// Created by alexx on 21.03.2016.
//

#ifndef QT_SECOND_GWTRPCREQUEST_H
#define QT_SECOND_GWTRPCREQUEST_H

#include "GwtResponseData.h"
#include <iostream>

namespace vantagefx {
    namespace api {
        class GwtRpcRequest {
        public:
	        explicit GwtRpcRequest(JsonVariantList items);
        private:
            int _version;
            int _flags;
            std::string _url;
            std::string _strongName;
            std::string _serviceName;
            std::string _methodName;
            std::vector<std::string> _parameterTypes;
            std::vector<std::string> _parameterValues;

            friend std::ostream &operator<<(std::ostream &stream, const vantagefx::api::GwtRpcRequest &request);
        };

        std::ostream &operator<<(std::ostream &stream, const vantagefx::api::GwtRpcRequest &request);
    }
}

#endif //QT_SECOND_GWTRPCREQUEST_H
