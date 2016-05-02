//
// Created by alexx on 08.04.2016.
//

#include "HttpConnection.h"
#include "HttpContext.h"
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/locale/encoding_utf.hpp>
#include <boost/lexical_cast.hpp>

#ifdef WINVER
#include <winhttp.h>
#endif

namespace vantagefx {
    namespace http {

        namespace pl = asio::placeholders;

        Connection::Connection(HttpContext &context, const std::string &scheme, const std::string &host, int port)
            : _scheme(scheme),
              _host(host),
              _port(port),
			  _context(context),
			  _resolver(context.service()),
			  _connected(false),
			  _busy(false)
        {
			std::string schemeAddress;

#ifdef WINVER
			std::string proxyAddress;
			WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxy;
			if (WinHttpGetIEProxyConfigForCurrentUser(&proxy)) {
				if (proxy.lpszProxy) {
					proxyAddress = boost::locale::conv::utf_to_utf<char>(proxy.lpszProxy);
					GlobalFree(proxy.lpszProxy);
				}
				if (proxy.lpszProxyBypass) {
					GlobalFree(proxy.lpszProxyBypass);
				}
				if (proxy.lpszAutoConfigUrl) {
					GlobalFree(proxy.lpszProxyBypass);
				}
			}
			enum State
			{
				Key,
				Value
			} state = Key;

			std::string key;
			std::string value;
			std::map<std::string, std::string> list;
			for (auto c : proxyAddress) {
				switch (state) {
				case Key:
					if (c == '=') state = Value;
					else key += c;
					break;
				case Value:
					if (c == ';') {
						list[key] = value;
						key = "";
						value = "";
						state = Key;
					}
					else value += c;
					break;
				}
			}
			if (!value.empty()) list[key] = value;
			if (list.size() == 0) {
				schemeAddress = proxyAddress;
			}
			else {
				auto it = list.find(scheme);
				if (it != list.end()) schemeAddress = it->second;
			}
#else
            char * data = getenv("https_proxy");
			if (data) schemeAddress = data;
#endif
			if (!schemeAddress.empty()) {
				Url proxyUrl(schemeAddress);
				_proxy = proxyUrl.host();
				_proxyPort = proxyUrl.port();
				auto url = host + ":" + boost::lexical_cast<std::string>(port);
				_proxyGreeting = "CONNECT " + url + " HTTP/1.1\r\n"+
					"Host: " + url + "\r\n" +
					"Connection: Keep-Alive\r\n" +
					"User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.112 Safari/537.36\r\n" +
					"\r\n";
			}
        }

        void Connection::send(HttpRequest &&request, ReadyHandler &&handler)
        {
            _busy = true;
            _request = std::move(request);
            _handler = std::move(handler);

            if (_connected) send();
            else open();
        }


        void Connection::closeConnection()
        {
			_context.closeConnection(shared_from_this());
        }

	    bool Connection::active()
        {
            return _connected && !_busy;
        }

	    std::string Connection::scheme() const
	    {
			return _scheme;
        }

        asio::mutable_buffers_1 Connection::buffer()
        {
            return asio::buffer(_buffer, _buffer.size());
        }

        bool Connection::processReceive(size_t bytes, bool proxy)
		{
            for (auto i = 0; i < bytes; i++) {
                if (_parser.advance(_buffer[i], _response) == Complete) {
                    _parser.reset();
					if (proxy) {
						_response = HttpResponse();
					}
					else {
						auto cookies = _response.headers("Set-Cookie");
						for (auto cookie : cookies) {
							_context.setCookie(cookie);
						}
						if (_request.close() || _response.has("Connection", "Close"))
							closeConnection();

						auto response = std::move(_response);
						_response = HttpResponse();

                        _handler(std::move(response), error_code());
                        _busy = false;
					}
                    return true;
                }
            }
            return false;
        }

        bool Connection::handleError(const error_code &ec)
        {
            if (ec) {
                _handler(std::move(_response), ec);
                return true;
            }
            return false;
        }


        void Connection::connect()
        {
            _connected = true;
        }

		bool Connection::disconnect()
		{
			return _connected.exchange(false);
		}

        const std::string &Connection::proxyGreeting() const
        {
	        return _proxyGreeting;
        }

	    std::string const & Connection::host() const
        {
	        return _host;
        }

	    int Connection::port() const
        {
	        return _port;
        }

	    bool Connection::isHandled(HttpRequest &req)
        {
			return active() && req.url().scheme() == scheme() && req.url().host() == host() &&
				(req.url().port() == port() || (req.url().port() == 0 && port() == 443));
        }

	    std::vector<boost::asio::const_buffer> Connection::requestBuffers()
        {
	        auto cookie = _context.cookieString(_request.url().path());
            _request.updateHeaders(cookie);
            return _request.toBuffers();
        }

	    ip::tcp::resolver::query Connection::endpoints() const
	    {
            if (_proxy.empty())
			    return ip::tcp::resolver::query(_host, boost::lexical_cast<std::string>(_port));
            else
                return ip::tcp::resolver::query(_proxy, boost::lexical_cast<std::string>(_proxyPort));
		}

	    /*
	    void HttpConnection::send(std::shared_ptr<HttpRequest> &request) {

	    }

	    void HttpConnection::close() {

	    }
	     */


        SslConnection::SslConnection(HttpContext &context, const std::string &host, int port)
                : Connection(context, "https", host, port == 0 ? 443 : port),
                  _socket(context.service(), context.ssl()),
                  _resolver(context.service())
        {}

	    SslConnection::~SslConnection()
        {
			SslConnection::close();
        }

	    void SslConnection::open()
        {
			auto self = shared_from_this();
			auto query = endpoints();

			_socket.set_verify_mode(ssl::verify_peer);

			_socket.set_verify_callback([this](bool preverified, ssl::verify_context &ctx) -> bool {
				return verifyCertificate(preverified, ctx);
			});

            _resolver.async_resolve(query, boost::bind(
                    &SslConnection::handleResolve, this, pl::error, pl::iterator));
		}

        void SslConnection::handleResolve(const error_code &ec, ip::tcp::resolver::iterator iterator)
		{
			auto self = shared_from_this();
            if (handleError(ec)) return;

			asio::async_connect(_socket.lowest_layer(), iterator, boost::bind(
                    &SslConnection::handleConnect, this, pl::error));
        }


        bool SslConnection::verifyCertificate(bool preverified, ssl::verify_context &context)
		{
            char subjectName[256];
            auto cert = X509_STORE_CTX_get_current_cert(context.native_handle());
            X509_NAME_oneline(X509_get_subject_name(cert), subjectName, 256);
            return preverified;
        }

        void SslConnection::handleConnect(const error_code &ec) 
		{
			auto self = shared_from_this();
            if (handleError(ec)) return;

			if (!proxyGreeting().empty()) {
				asio::async_write(_socket.next_layer(), asio::buffer(proxyGreeting()), boost::bind(
					&SslConnection::handleProxyWrite, this, pl::error));
			}
			else {
				_socket.async_handshake(ssl::stream_base::client, boost::bind(
					&SslConnection::handleHandshake, this, pl::error));
			}
        }

	    void SslConnection::handleProxyWrite(const error_code &ec)
        {
			auto self = shared_from_this();
            if (handleError(ec)) return;

			_socket.next_layer().async_read_some(buffer(), boost::bind(
				&SslConnection::handleProxyRead, this, pl::error, pl::bytes_transferred));
        }

	    void SslConnection::handleProxyRead(const error_code &ec, size_t bytesTransferred)
        {
			auto self = shared_from_this();
            if (handleError(ec)) return;

            if (processReceive(bytesTransferred, true)) {
                _socket.async_handshake(ssl::stream_base::client, boost::bind(
                        &SslConnection::handleHandshake, this, pl::error));
            }
            else {
                _socket.next_layer().async_read_some(buffer(), boost::bind(
                        &SslConnection::handleProxyRead, this, pl::error, pl::bytes_transferred));
            }
		}

	    void SslConnection::handleHandshake(const error_code &ec) 
		{
			auto self = shared_from_this();
            if (handleError(ec)) return;

            connect();
            send();
        }

        void SslConnection::send()
        {
            asio::async_write(_socket, requestBuffers(), boost::bind(
                    &SslConnection::handleWrite, this, pl::error));
        }

        void SslConnection::close() {
			if(disconnect()) {
				error_code ec;
				_socket.lowest_layer().shutdown(ip::tcp::socket::shutdown_receive, ec);
			}
        }

		void SslConnection::handleWrite(const error_code &ec)
		{
			auto self = shared_from_this();
            if (handleError(ec)) return;

            _socket.async_read_some(buffer(), boost::bind(
                    &SslConnection::dataReceived, this, pl::error, pl::bytes_transferred));
		}

		void SslConnection::dataReceived(const error_code &ec, size_t bytesTransferred)
		{
			auto self = shared_from_this();
			if (ec) {
				if (ec.category() == asio::error::get_ssl_category() &&
					ec.value() == ERR_PACK(ERR_LIB_SSL, 0, SSL_R_SHORT_READ)) {
					_socket.lowest_layer().close();
				}
                handleError(ec);
				return;
            }

            if (processReceive(bytesTransferred)) return;

            _socket.async_read_some(buffer(), boost::bind(
                    &SslConnection::dataReceived, this, pl::error, pl::bytes_transferred));
		}
	}
}
