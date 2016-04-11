//
// Created by alexx on 08.04.2016.
//

#include "HttpResponse.h"
#include <boost/algorithm/string.hpp>

namespace vantagefx {
    namespace http {

        std::string const &HttpResponse::body() const
        {
            return _body;
        }

        void HttpResponse::setBody(std::string body)
        {
            _body = body;
        }

        void HttpResponse::addHeader(std::string key, std::string value)
        {
            _headers.push_back(std::make_pair(key, value));
        }

        std::string HttpResponse::header(std::string key) const
        {
            for(auto pair: _headers) {
				if (boost::iequals(pair.first, key)) return pair.second;
            }
            return std::string();
        }

	    std::vector<std::string> HttpResponse::headers(std::string key) const
        {
			std::vector<std::string> result;
			for (auto pair : _headers) {
				if (boost::iequals(pair.first, key)) 
					result.push_back(pair.second);
			}
			return result;
		}


	    void HttpResponse::setCode(std::string code)
        {
            _code = code;
        }

        std::string const &HttpResponse::code() const
        {
            return _code;
        }

        void HttpResponse::setCodeDescription(std::string description)
        {
            _codeDescription = description;
        }

        std::string const &HttpResponse::codeDescription() const
        {
            return _codeDescription;
        }

        void HttpResponse::setMajor(int major)
        {
            _major = major;
        }

        int HttpResponse::major() const
        {
            return _major;
        }

        void HttpResponse::setMinor(int minor)
        {
            _minor = minor;
        }

        int HttpResponse::minor() const
        {
            return _minor;
        }
    }
}


