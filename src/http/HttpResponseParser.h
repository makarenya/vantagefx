//
// Created by alexx on 08.04.2016.
//

#ifndef VANTAGEFX_HTTPRESPONSEPARSER_H
#define VANTAGEFX_HTTPRESPONSEPARSER_H

#include "HttpResponse.h"

namespace vantagefx {
    namespace http {

        enum ParserState
        {
            Invalid,
            Intermediate,
            Complete
        };

        enum HttpResponseState {
            Initial,
            T1,
            T2,
            P,
            Slash,
            Pnt,
            Major,
            Minor,
            VersionEnd,
            Code,
            CodeDescription,
            Lf,
            HeaderName,
            HeaderValue,
            EndLf,
            Body,
        };

        class HttpResponseParser
        {
        public:
            HttpResponseParser();
            ParserState advance(char c, HttpResponse &response);
	        void reset();
        private:
            HttpResponseState _state;
            std::string _key;
            std::string _value;
            size_t _length;
        };
    }
}


#endif //VANTAGEFX_HTTPRESPONSEPARSER_H
