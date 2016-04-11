//
// Created by alexx on 08.04.2016.
//

#ifndef VANTAGEFX_HTTPRESPONSE_H
#define VANTAGEFX_HTTPRESPONSE_H

#include <string>
#include <vector>


namespace vantagefx {
    namespace http {

        class HttpResponse
        {
        public:
            void setBody(std::string body);
            std::string const &body() const;
            void addHeader(std::string key, std::string value);
            std::string header(std::string key) const;
			std::vector<std::string> headers(std::string key) const;
            void setCode(std::string code);
            std::string const &code() const;
            void setCodeDescription(std::string description);
            std::string const &codeDescription() const;
            void setMajor(int major);
            int major() const;
            void setMinor(int minor);
            int minor() const;

        private:
            std::string _body;
            std::string _code;
            std::string _codeDescription;
            std::vector<std::pair<std::string, std::string>> _headers;
            int _major;
            int _minor;
        };
    }
}


#endif //VANTAGEFX_HTTPRESPONSE_H
