//
// Created by alexx on 21.03.2016.
//

#include "GwtRpcResponse.h"

namespace vantagefx {
    namespace api {

        GwtRpcResponse::GwtRpcResponse(int version, int flags, JsonVariantList data, StringList stringTable) {
            using boost::get;

            std::vector<std::string> strings;
            strings.push_back(std::string());
            for (auto value: stringTable) {
                strings.push_back(value);
            }
            _version = version;
            if (_version != 7) {
                throw std::runtime_error("only version 7 is allowed");
            }
            _flags = static_cast<GwtFlags>(flags);
            auto current = data.end();
	        auto type = strings[get<int>(*--current)];
        }
    }
}

