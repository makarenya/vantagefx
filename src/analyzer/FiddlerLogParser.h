//
// Created by AMakarenya on 30.03.2016.
//

#ifndef VANTAGEFX_FIDDLERLOGPARSER_H
#define VANTAGEFX_FIDDLERLOGPARSER_H

#include "FiddlerLogEntry.h"
#include <string>
#include <stdint.h>

namespace vantagefx {
    namespace analyzer {

        class FiddlerLogParser {
        public:

	        explicit FiddlerLogParser();

            enum Status {
                Intermediate,
                Complete,
                Invalid
            };

            Status advance(uint8_t c);
			FiddlerLogEntry const &result() const { return _result; }
			bool lastComplete() 
			{
				if(_state == ResponseBodyCr) {
					_state = Method;
					std::swap(_result, _current);
					_current = FiddlerLogEntry();
					return true;
				}
				return false;
			}

        private:
            enum ParseState {
                Method,
                Url,
                RequestH,
                RequestT1,
                RequestT2,
                RequestP,
                RequestSlash,
                RequestPnt,
                RequestMajor,
                RequestMinor,
                RequestCr,
                RequestLf,
                RequestHeaderName,
                RequestHeaderValue,
                HeadersEndLf,
                RequestBody,
                RequestBodyCr,
                RequestBodyLf,
                ResponseH,
                ResponseT1,
                ResponseT2,
                ResponseP,
                ResponseSlash,
                ResponsePnt,
                ResponseMajor,
                ResponseMinor,
                ResponseVersionEnd,
                ResponseCode,
                ResponseCodeDescription,
                ResponseLf,
                ResponseHeaderName,
                ResponseHeaderValue,
                ResponseEndLf,
                ResponseBodyCr,
                ResponseBodyLf,
                ResponseBody,
                EntryCr,
                EntryLf,
                EntrySeparator,
                EntryCr2,
                EntryLf2,
                EntryCr3,
                EntryLf3,
            };

            ParseState _state;
            FiddlerLogEntry _current;
			FiddlerLogEntry _result;
            std::string _key;
            std::string _value;
            int _length;
        };
    }
}

#endif //VANTAGEFX_FIDDLERLOGPARSER_H
