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

	    HttpResponse::HttpResponse()
			: _major(0),
	          _minor(0)
	    {}

	    HttpResponse::HttpResponse(HttpResponse &&rhs)
	    {
		    _body = std::move(rhs._body);
		    _code = std::move(rhs._code);
		    _codeDescription = std::move(rhs._codeDescription);
		    _headers = std::move(rhs._headers);
		    _major = rhs._major;
		    _minor = rhs._minor;
	    }

	    HttpResponse & HttpResponse::operator=(HttpResponse &&rhs)
	    {
		    if (&rhs != this) {
			    _body = std::move(rhs._body);
			    _code = std::move(rhs._code);
			    _codeDescription = std::move(rhs._codeDescription);
			    _headers = std::move(rhs._headers);
			    _major = rhs._major;
			    _minor = rhs._minor;
		    }
		    return *this;
	    }

	    void HttpResponse::setBody(const std::string &body)
        {
            _body = body;
        }

        void HttpResponse::addHeader(const std::string &key, const std::string &value)
        {
            _headers.push_back(std::make_pair(key, value));
        }

        std::string HttpResponse::header(const std::string &key) const
        {
            for(auto pair: _headers) {
				if (boost::iequals(pair.first, key)) return pair.second;
            }
            return std::string();
        }

	    std::vector<std::string> HttpResponse::headers(const std::string &key) const
        {
			std::vector<std::string> result;
			for (auto pair : _headers) {
				if (boost::iequals(pair.first, key)) 
					result.push_back(pair.second);
			}
			return result;
		}

		bool HttpResponse::has(const std::string &key, const std::string &value) const
        {
	        for (auto pair :_headers) {
				if (boost::iequals(pair.first, key) &&
					boost::iequals(pair.second, value)) return true;
	        }
			return false;
        }


	    void HttpResponse::setCode(const std::string &code)
        {
            _code = code;
        }

        std::string const &HttpResponse::code() const
        {
            return _code;
        }

        void HttpResponse::setCodeDescription(const std::string &description)
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


