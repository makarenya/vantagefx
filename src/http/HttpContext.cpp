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
			connection->close();
			auto it = std::find_if(_connections.begin(), _connections.end(), [&connection](const ConnectionPtr &item)
			{
				return item == connection;
			});
			_connections.erase(it);
		}

	    void HttpContext::send(HttpRequestPtr req) {
            ConnectionPtr connection;
			auto &host = req->url().host;
	        auto port = req->url().port;

            if (req->url().scheme == Https) {
                for (auto cn :_connections) {
                    if (cn->active() && cn->scheme() == Https && cn->host() == host) {
                        connection = cn;
                        connection->send(req);
                        return;
                    }
                }
                connection = std::make_shared<SslConnection>(*this, host, port);
                connection->open([this, connection, req](const error_code &ec) {
					if (ec) {
						connection->closeConnection();
						req->setError(ec);
					}
                    else connection->send(req);
                });
                _connections.push_back(connection);
            }
        }

	    void HttpContext::get(std::string url, std::function<void(HttpRequestPtr request)> handler)
        {
			auto request = std::make_shared<HttpRequest>(url);
			request->setHandler(handler);
			send(request);
        }

	    boost::asio::io_service & HttpContext::service() const
        {
	        return _io_service;
        }

	    boost::asio::ssl::context & HttpContext::ssl() const
        {
	        return _context;
        }
    }
}
