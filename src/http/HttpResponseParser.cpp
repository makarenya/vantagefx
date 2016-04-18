//
// Created by alexx on 08.04.2016.
//

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "HttpResponseParser.h"

namespace vantagefx {
    namespace http {

        HttpResponseParser::HttpResponseParser()
                : _state(Initial),
                  _length(0)
        {}

        ParserState HttpResponseParser::advance(char c, HttpResponse &response)
        {
            switch(_state) {
                case Initial:
                    if (c == 'H') _state = T1;
                    else return Invalid;
                    break;
                case T1:
                    if (c == 'T') _state = T2;
                    else return Invalid;
                    break;
                case T2:
                    if (c == 'T') _state = P;
                    else return Invalid;
                    break;
                case P:
                    if (c == 'P') _state = Slash;
                    else return Invalid;
                    break;
                case Slash:
                    if (c == '/') _state = Major;
                    else return Invalid;
                    break;
                case Major:
                    if (c >= '1' && c <= '9') {
                        response.setMajor(c - '0');
                        _state = Pnt;
                    }
                    else return Invalid;
                    break;
                case Pnt:
                    if (c == '.') _state = Minor;
                    else return Invalid;
                    break;
                case Minor:
                    if (c >= '1' && c <= '9') {
                        response.setMinor(c - '0');
                        _state = VersionEnd;
                    }
                    else return Invalid;
                    break;
                case VersionEnd:
                    if (c == ' ') _state = Code;
                    else return Invalid;
                    break;
                case Code:
                    if (c == ' ') {
                        _state = CodeDescription;
                        response.setCode(_value);
                        _value = "";
                    }
                    else if (c > ' ') _value += c;
                    else return Invalid;
                    break;
                case CodeDescription:
                    if (c == '\r') {
                        _state = Lf;
                        response.setCodeDescription(_value);
                        _value = "";
                    }
                    else if (c >= ' ') _value += c;
                    else return Invalid;
                    break;
                case Lf:
                    if (c == '\n') _state = HeaderName;
                    else return Invalid;
                    break;
                case HeaderName:
                    if (c == ':') _state = HeaderValue;
                    else if (c == '\r') _state = EndLf;
                    else if (c >= ' ') _key += c;
                    else return Invalid;
                    break;
                case HeaderValue:
                    if (c == '\r') {
                        _state = Lf;
                        response.addHeader(boost::trim_copy(_key), boost::trim_copy(_value));
                        _key = "";
                        _value = "";
                    }
                    else if (c >= ' ') _value += c;
                    else return Invalid;
                    break;
                case EndLf:
                    if (c == '\n') {
                        auto cl = response.header("Content-Length");
                        if (cl.empty()) _length = 0;
                        else _length = boost::lexical_cast<size_t>(cl);
                        if (_length == 0) {
							_state = Initial;
							return Complete;
                        }
                        _state = Body;
                    }
                    else return Invalid;
                    break;
                case Body:
                    _value += c;
                    if (--_length == 0) {
                        _state = Initial;
                        response.setBody(_value);
                        _value = "";
						return Complete;
                    }
                    break;
            }
            return Intermediate;
        }

	    void HttpResponseParser::reset()
        {
			_state = Initial;
			_key = "";
			_value = "";
			_length = 0;
		}
    }
}

