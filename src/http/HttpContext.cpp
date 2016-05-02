//
// Created by alexx on 08.04.2016.
//

#include <iostream>
#include <boost/bind.hpp>
#include "HttpContext.h"
#include <boost/algorithm/string.hpp>
#include <src/serialized/State.h>
#include <boost/regex.hpp>
#include "HttpConnection.h"

namespace vantagefx {
    namespace http {

        namespace parser {

            enum SetCookieParseState {
                Key,
                Value,
                Parameter,
                ParameterValue
            };

            class SetCookieParser {
            public:

                SetCookieParser()
                        : _state(Key) { }

                void advance(char c) {
                    switch (_state) {
                        case Key:
                            if (c == '=') _state = Value;
                            else _key += c;
                            break;
                        case Value:
                            if (c == ';') _state = Parameter;
                            else _cookie.first += c;
                            break;
                        case Parameter:
                            if (c == '=') _state = ParameterValue;
                            else if (c == ';') addParam();
                            else _pkey += c;
                            break;
                        case ParameterValue:
                            if (c == ';') {
                                addParam();
                                _state = Parameter;
                            }
                            else _pvalue += c;
                            break;
                    }
                }

                void addParam() {
                    boost::trim(_pkey);
                    boost::to_lower(_pkey);
                    boost::trim(_pvalue);
                    _cookie.second[_pkey] = _pvalue;
                    _pkey = "";
                    _pvalue = "";
                }

                void finalize() {
                    if (!_pkey.empty()) addParam();
                    _state = Key;
                }

                std::string const &key() const { return _key; }

                HttpContext::Cookie const &cookie() const { return _cookie; }

            private:
                std::string _key;
                HttpContext::Cookie _cookie;
                SetCookieParseState _state;
                std::string _pkey;
                std::string _pvalue;
            };
        }

        HttpContext::HttpContext(boost::asio::io_service &io_service, boost::asio::ssl::context &context)
                : _io_service(io_service),
                  _context(context) { }

        void HttpContext::setCookie(std::string cookie) {
            parser::SetCookieParser p;
            for (auto c :cookie) {
                p.advance(c);
            }
            p.finalize();
            _cookies[boost::trim_copy(p.key())] = p.cookie();
        }

        std::string HttpContext::cookieString(std::string path) const {
            std::string result = "";
            for (auto pair: _cookies) {
                auto it = pair.second.second.find("path");
                auto cookiePath = (it == pair.second.second.end()) ? "/" : it->second;
                if (boost::starts_with(path, cookiePath)) {
                    if (!result.empty()) result += "; ";
                    result += pair.first + "=" + pair.second.first;
                }
            }
            return result;
        }

	    void HttpContext::closeConnection(ConnectionPtr connection)
        {
            _connections.erase(connection);
            connection->close();
		}

        void HttpContext::send(HttpRequest &&request, ReadyHandler &&handler) {
            ConnectionPtr connection;

			auto found = std::find_if(_connections.begin(), _connections.end(), [&request](ConnectionPtr cn) {
				return cn->isHandled(request);
			});
			if (found == _connections.end()) {
                connection = std::make_shared<SslConnection>(std::ref(*this), request.url().host(), request.url().port());
				_connections.insert(connection);
			}
            else {
                connection = *found;
            }
			
            connection->send(std::move(request), std::move(handler));
		}

		struct SendHandler
        {
			explicit SendHandler(std::promise<HttpResponse> &&promise)
			{
				_promise = std::make_shared<std::promise<HttpResponse>>(std::move(promise));
			}

			void operator()(HttpResponse response, const error_code &ec) const
			{
				if (ec)
					_promise->set_exception(std::make_exception_ptr(boost::system::system_error(ec)));
				else
					_promise->set_value(std::move(response));
			}
			std::shared_ptr<std::promise<HttpResponse>> _promise;
        };

	    std::future<HttpResponse> HttpContext::send(HttpRequest &&request)
        {
			std::promise<HttpResponse> promise;
			auto future = promise.get_future();
			send(std::move(request), SendHandler(std::move(promise)));
			return future;
		}


	    boost::asio::io_service & HttpContext::service() const
        {
	        return _io_service;
        }

	    boost::asio::ssl::context & HttpContext::ssl() const
        {
	        return _context;
        }

        void HttpContext::stop() {
            for(auto connection : _connections) {
                connection->close();
            }
            _connections.clear();
        }
    }
}
