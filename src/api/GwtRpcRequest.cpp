//
// Created by alexx on 21.03.2016.
//

#include "GwtRpcRequest.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>

namespace vantagefx {
    namespace api {

        GwtRpcRequest::GwtRpcRequest(JsonVariantList items) {
            using boost::get;
            auto item = items.cbegin();
            _version = get<int>(*item++);
            _flags = get<int>(*item++);
            auto stringsCount = get<int>(*item++);
            std::vector<std::string> strings;
            strings.push_back("");
            for (auto i = 0; i < stringsCount; i++) {
                strings.push_back(get<std::string>(*item++));
            }
            _url = strings[get<int>(*item++)];
            _strongName = strings[get<int>(*item++)];
            if (_flags && RpcTokenIcluded) {
                throw new std::runtime_error("rpc token not supported");
            }
            _serviceName = strings[get<int>(*item++)];
            _methodName = strings[get<int>(*item++)];
            int parametersCount = get<int>(*item++);
            for (int i = 0; i < parametersCount; i++) {
                auto value = strings[get<int>(*item++)];
                std::vector<std::string> parts;
                boost::algorithm::split(parts, value, boost::algorithm::is_any_of("/"));
                _parameterTypes.push_back(parts[0]);
            }
            for (auto type: _parameterTypes) {
                if (type == "Z") {
                    _parameterValues.push_back(get<int>(*item++) == 0 ? "false" : "true");
                }
                else if (type == "B" || type == "C" || type == "S" || type == "T") {
                    _parameterValues.push_back(boost::lexical_cast<std::string>(get<int>(*item++)));
                }
                else if (type == "J") {
                    auto val = get<std::string>(*item++);
                    int64_t result = 0;
                    for (auto c: val) {
                        result *= 64;
                        if (c >= 'A' && c <= 'Z') {
                            result += c - 'A';
                        }
                        else if (c >= 'a' && c <= 'z') {
                            result += c - 'a' + 26;
                        }
                        else if (c >= '0' && c <= '9') {
                            result += c - '0' + 52;
                        }
                        else if (c == '$') result += 62;
                        else if (c == '_') result += 63;
                        else if (c != '=') {
                            throw new std::runtime_error("invalid base64 symbol");
                        }
                    }
                    std::stringstream stream;
                    stream << "0x" << std::hex << result;
                    _parameterValues.push_back(stream.str());
                }
                else if (type == "F" || type == "D") {
                    _parameterValues.push_back(boost::lexical_cast<std::string>(get<double>(*item++)));
                }
                else if (type == "java.lang.String") {
                    _parameterValues.push_back(strings[get<int>(*item++)]);
                }
            }
        }

        std::ostream &operator<<(std::ostream &stream, const vantagefx::api::GwtRpcRequest &request) {
            stream << request._url << " " << request._serviceName << "::" << request._methodName << "(";
            auto types = request._parameterTypes;
            auto values = request._parameterValues;
            for (auto i = 0; i < types.size(); i++) {
                if (i != 0) stream << ", ";
                if (types[i] == "Z") stream << "bool ";
                else if (types[i] == "B") stream << "byte: " << values[i];
                else if (types[i] == "C") stream << "char: " << values[i];
                else if (types[i] == "S") stream << "short: " << values[i];
                else if (types[i] == "I") stream << "int: " << values[i];
                else if (types[i] == "J") stream << "long: " << values[i];
                else if (types[i] == "F") stream << "float: " << values[i];
                else if (types[i] == "D") stream << "double: " << values[i];
                else stream << types[i] << ": '" << values[i] << "'";
            }
            return stream << ")";
        }
    }
}


