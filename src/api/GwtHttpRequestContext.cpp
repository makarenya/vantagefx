//
// Created by alexx on 29.04.2016.
//

#include "GwtHttpRequestContext.h"
#include <boost/lexical_cast.hpp>

namespace vantagefx {
    namespace api {
		namespace base64 {

			inline char base64Value(int b) {
				if (b < 26) {
					return b + 'A';
				}
				if (b < 52) {
					return b - 26 + 'a';
				}
				if (b < 62) {
					return b - 52 + '0';
				}
				if (b == 62) return '$';
				if (b == 63) return '_';
				throw std::runtime_error("invalid base64 symbol");
			}

			std::string base64Encode(int64_t val)
			{
				std::string result;
				if (val == 0) return "A";
				while (val > 0) {
					result = base64Value(val & 0x3f) + result;
					val >>= 6;
				}
				return result;
			}
		}

        GwtHttpRequestContext &GwtHttpRequestContext::operator<<(int value)
        {
            _data.push_back(boost::lexical_cast<std::string>(value));
            return *this;
        }

        GwtHttpRequestContext &GwtHttpRequestContext::operator<<(int64_t value)
        {
            _data.push_back(base64::base64Encode(value));
            return *this;
        }

        GwtHttpRequestContext &GwtHttpRequestContext::operator<<(double value)
        {
            _data.push_back(boost::lexical_cast<std::string>(value));
            return *this;
        }

        GwtHttpRequestContext &GwtHttpRequestContext::operator<<(std::string value)
        {
            if (value.empty()) {
                return *this << 0;
            }
            for (auto i = 0; i < static_cast<int>(_strings.size()); ++i) {
                if (_strings[i] == value) {
                    return *this << (i + 1);
                }
            }
            _strings.push_back(value);
            return *this << static_cast<int>(_strings.size());
        }

        std::string GwtHttpRequestContext::str() const
        {
            std::stringstream result;
            result << "7|0|" << static_cast<int>(_strings.size()) << "|";
            for(auto str :_strings) {
                result << str << "|";
            }
            for(auto item : _data) {
                result << item << "|";
            }
            return result.str();
        }
    }
}