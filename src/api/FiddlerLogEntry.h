//
// Created by AMakarenya on 30.03.2016.
//

#ifndef VANTAGEFX_FIDDLERLOGENTRY_H
#define VANTAGEFX_FIDDLERLOGENTRY_H

#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>

namespace vantagefx {
    namespace api {

        class FiddlerLogEntry {
        public:
			FiddlerLogEntry()
				: _requestMajor(0),
				_requestMinor(0),
				_responseMajor(0),
				_responseMinor(0)
			{ }

			void addRequestHeader(std::string key, std::string value);
			void addResponseHeader(std::string key, std::string value);
			void clearHeaders();
			std::string requestHeader(std::string name);
	        std::string responseHeader(std::string name);

	        void setMethod(std::string method);
			std::string method() const { return _method; }

			void setUrl(std::string url);
			std::string url() const { return _url; }

	        void setRequestMajor(int value);
			int requestMajor() const { return _requestMajor; }

	        void setRequestMinor(int value);
			int requestMinor() const { return _requestMinor; }

	        void setResponseMajor(int value);
			int responseMajor() const { return _responseMajor; }

	        void setResponseMinor(int value);
			int responseMinor() const { return _responseMinor; }

	        void setRequest(std::string value);
			std::string request() const { return _request; }

	        void setResponse(std::string value);
			std::string response() const { return _response; }

	        void setCode(std::string value);
			std::string code() const { return _code; }

	        void setCodeDescription(std::string value);
			std::string codeDescription() const { return _codeDescription; }

        private:
            std::string _method;
            std::string _url;
            std::vector<std::pair<std::string, std::string>> _requestHeaders;
            std::vector<std::pair<std::string, std::string>> _responseHeaders;
            int _requestMajor;
            int _requestMinor;
            int _responseMajor;
            int _responseMinor;
            std::string _request;
            std::string _response;
            std::string _code;
            std::string _codeDescription;
        };
    }
}


#endif //VANTAGEFX_FIDDLERLOGENTRY_H
