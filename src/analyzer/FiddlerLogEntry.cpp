//
// Created by AMakarenya on 30.03.2016.
//

#include "FiddlerLogEntry.h"

namespace vantagefx {
    namespace analyzer {

	    std::string FiddlerLogEntry::requestHeader(std::string name)
	    {
		    for (auto pair: _requestHeaders) {
			    if (boost::iequals(std::get<0>(pair), name)) return std::get<1>(pair);
		    }
		    return "";
	    }

	    std::string FiddlerLogEntry::responseHeader(std::string name)
	    {
		    for (auto pair: _responseHeaders) {
			    if (boost::iequals(std::get<0>(pair), name)) return std::get<1>(pair);
		    }
		    return "";
	    }

	    void FiddlerLogEntry::setMethod(std::string method)
	    {
		    _method = method;
	    }

	    void FiddlerLogEntry::setUrl(std::string url)
	    {
		    _url = url;
	    }

	    void FiddlerLogEntry::addRequestHeader(std::string key, std::string value)
	    {
		    _requestHeaders.push_back(std::make_pair(boost::trim_copy(key), boost::trim_copy(value)));
	    }

	    void FiddlerLogEntry::addResponseHeader(std::string key, std::string value)
	    {
		    _responseHeaders.push_back(std::make_pair(boost::trim_copy(key), boost::trim_copy(value)));
	    }

	    void FiddlerLogEntry::clearHeaders()
	    {
			_requestHeaders.clear();
			_responseHeaders.clear();
	    }

	    void FiddlerLogEntry::setRequestMajor(int value)
	    {
		    _requestMajor = value;
	    }

	    void FiddlerLogEntry::setRequestMinor(int value)
	    {
		    _requestMinor = value;
	    }

	    void FiddlerLogEntry::setResponseMajor(int value)
	    {
		    _responseMajor = value;
	    }

	    void FiddlerLogEntry::setResponseMinor(int value)
	    {
		    _responseMinor = value;
	    }

	    void FiddlerLogEntry::setRequest(std::string value)
	    {
		    _request = value;
	    }

	    void FiddlerLogEntry::setResponse(std::string value)
	    {
		    _response = value;
	    }

	    void FiddlerLogEntry::setCode(std::string value)
	    {
		    _code = value;
	    }

	    void FiddlerLogEntry::setCodeDescription(std::string value)
	    {
		    _codeDescription = value;
	    }
    }
}