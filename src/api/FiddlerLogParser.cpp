//
// Created by AMakarenya on 30.03.2016.
//

#include "FiddlerLogParser.h"
#include <boost/lexical_cast.hpp>

namespace vantagefx {
    namespace api {

        FiddlerLogParser::FiddlerLogParser()
                : _state(Method),
                  _length(0)
		{ }

		FiddlerLogParser::Status FiddlerLogParser::advance(uint8_t c) {
            switch (_state) {
                case Method:
                    if (c == ' ') {
                        _state = Url;
                        _current.setMethod(_value);
                        _value = "";
                    }
                    else if (c > ' ') _value += c;
                    else return Invalid;
                    break;
                case Url:
                    if (c == ' ') {
                        _state = RequestH;
                        _current.setUrl(_value);
                        _value = "";
                    }
                    else if (c > ' ') _value += c;
                    else return Invalid;
                    break;
                case RequestH:
                    if (c == 'H') _state = RequestT1;
                    else return Invalid;
                    break;
                case RequestT1:
                    if (c == 'T') _state = RequestT2;
                    else return Invalid;
                    break;
                case RequestT2:
                    if (c == 'T') _state = RequestP;
                    else return Invalid;
                    break;
                case RequestP:
                    if (c == 'P') _state = RequestSlash;
                    else return Invalid;
                    break;
                case RequestSlash:
                    if (c == '/') _state = RequestMajor;
                    else return Invalid;
                    break;
                case RequestMajor:
                    if (c >= '1' && c <= '9') {
                        _current.setRequestMajor(c - '0');
                        _state = RequestPnt;
                    }
                    else return Invalid;
                    break;
                case RequestPnt:
                    if (c == '.') _state = RequestMinor;
                    else return Invalid;
                    break;
                case RequestMinor:
                    if (c >= '1' && c <= '9') {
                        _current.setRequestMinor(c - '0');
                        _state = RequestCr;
                    }
                    else return Invalid;
                    break;
                case RequestCr:
                    if (c == '\r') _state = RequestLf;
                    else return Invalid;
                    break;
                case RequestLf:
                    if (c == '\n') _state = RequestHeaderName;
                    else return Invalid;
                    break;
                case RequestHeaderName:
                    if (c == ':') _state = RequestHeaderValue;
                    else if (c == '\r') _state = HeadersEndLf;
                    else if (c >= ' ') _key += c;
                    else return Invalid;
                    break;
                case RequestHeaderValue:
                    if (c == '\r') {
                        _state = RequestLf;
                        _current.addRequestHeader(_key, _value);
                        _key = "";
                        _value = "";
                    }
                    else if (c >= ' ') _value += c;
                    else return Invalid;
                    break;
                case HeadersEndLf:
                    if (c == '\n') {
                        auto cl = _current.requestHeader("Content-Length");
                        if (cl.empty()) _length = 0;
                        else _length = boost::lexical_cast<int>(cl);
                        if (_length == 0) _state = RequestBodyCr;
                        else _state = RequestBody;
                    }
                    else return Invalid;
                    break;
                case RequestBody:
					_value += c;
					if (--_length == 0) {
                        _state = RequestBodyCr;
                        _current.setRequest(_value);
                        _value = "";
                    }
                    break;
                case RequestBodyCr:
                    if (c == '\r') _state = RequestBodyLf;
                    else return Invalid;
                    break;
                case RequestBodyLf:
                    if (c == '\n') _state = ResponseH;
                    else return Invalid;
                    break;
                case ResponseH:
                    if (c == 'H') _state = ResponseT1;
                    else return Invalid;
                    break;
                case ResponseT1:
                    if (c == 'T') _state = ResponseT2;
                    else return Invalid;
                    break;
                case ResponseT2:
                    if (c == 'T') _state = ResponseP;
                    else return Invalid;
                    break;
                case ResponseP:
                    if (c == 'P') _state = ResponseSlash;
                    else return Invalid;
                    break;
                case ResponseSlash:
                    if (c == '/') _state = ResponseMajor;
                    else return Invalid;
                    break;
                case ResponseMajor:
                    if (c >= '1' && c <= '9') {
                        _current.setResponseMajor(c - '0');
                        _state = ResponsePnt;
                    }
                    else return Invalid;
                    break;
                case ResponsePnt:
                    if (c == '.') _state = ResponseMinor;
                    else return Invalid;
                    break;
                case ResponseMinor:
                    if (c >= '1' && c <= '9') {
                        _current.setResponseMinor(c - '0');
                        _state = ResponseVersionEnd;
                    }
                    else return Invalid;
                    break;
                case ResponseVersionEnd:
                    if (c == ' ') _state = ResponseCode;
                    else return Invalid;
                    break;
                case ResponseCode:
                    if (c == ' ') {
                        _state = ResponseCodeDescription;
                        _current.setCode(_value);
                        _value = "";
                    }
                    else if (c > ' ') _value += c;
                    else return Invalid;
                    break;
                case ResponseCodeDescription:
                    if (c == '\r') {
                        _state = ResponseLf;
                        _current.setCodeDescription(_value);
                        _value = "";
                    }
                    else if (c >= ' ') _value += c;
                    else return Invalid;
                    break;
                case ResponseLf:
                    if (c == '\n') _state = ResponseHeaderName;
                    else return Invalid;
                    break;
                case ResponseHeaderName:
                    if (c == ':') _state = ResponseHeaderValue;
                    else if (c == '\r') _state = ResponseEndLf;
                    else if (c >= ' ') _key += c;
                    else return Invalid;
                    break;
                case ResponseHeaderValue:
                    if (c == '\r') {
                        _state = ResponseLf;
                        _current.addResponseHeader(_key, _value);
                        _key = "";
                        _value = "";
                    }
                    else if (c >= ' ') _value += c;
                    else return Invalid;
                    break;
                case ResponseEndLf:
                    if (c == '\n') {
                        auto cl = _current.responseHeader("Content-Length");
                        if (cl.empty()) _length = 0;
                        else _length = boost::lexical_cast<int>(cl);
                        if (_length == 0) _state = ResponseBodyCr;
                        else _state = ResponseBody;
                    }
                    else return Invalid;
                    break;
                case ResponseBody:
					_value += c;
					if (--_length == 0) {
                        _state = ResponseBodyCr;
                        _current.setResponse(_value);
                        _value = "";
                    }
                    break;
                case ResponseBodyCr:
                    if (c == '\r') _state = ResponseBodyLf;
                    else return Invalid;
                    break;
                case ResponseBodyLf:
                    if (c == '\n') _state = EntryCr;
                    else return Invalid;
                    break;
                case EntryCr:
                    if (c == '\r') _state = EntryLf;
                    else return Invalid;
                    break;
                case EntryLf:
                    if (c == '\n') {
                        _length = 66;
                        _state = EntrySeparator;
                    }
                    else return Invalid;
                    break;
                case EntrySeparator:
                    if (--_length == 0) _state = EntryCr2;
                    else if (c != '-') return Invalid;
                    break;
                case EntryCr2:
                    if (c == '\r') _state = EntryLf2;
                    else return Invalid;
                    break;
                case EntryLf2:
                    if (c == '\n') _state = EntryCr3;
                    else return Invalid;
                    break;
                case EntryCr3:
                    if (c == '\r') _state = EntryLf3;
                    else return Invalid;
                    break;
                case EntryLf3:
                    if (c == '\n') {
                        _state = Method;
						std::swap(_result, _current);
						_current = FiddlerLogEntry();
                        return Complete;
                    }
                    else return Invalid;
            }
            return Intermediate;
        }
    }
}