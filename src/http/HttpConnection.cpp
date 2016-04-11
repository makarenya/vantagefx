//
// Created by alexx on 08.04.2016.
//

#include "HttpConnection.h"
#include "HttpContext.h"
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <boost/lexical_cast.hpp>
#include <iostream>

namespace vantagefx {
    namespace http {

        Connection::Connection(HttpContext &context, std::string host, int port)
            : _host(host),
			  _port(port),
              _context(context),
			  _resolver(context.service()),
			  _connected(false)
		{}


	    void Connection::closeConnection()
        {
			_context.closeConnection(shared_from_this());
        }

	    bool Connection::active() {
            return _connected && !_request;
        }

        void Connection::setRequest(HttpRequestPtr &request)
		{
            _request = request;
        }

        void Connection::setResponse(HttpResponse &response) {
			auto self = shared_from_this();
			if (_request->close()) {
				closeConnection();
			}
			_request->setResponse(response);
			_request.reset();
        }

        void Connection::setError(const error_code &ec) {
			auto self = shared_from_this();
			closeConnection();
			_request->setError(ec);
			_request.reset();
        }

        boost::asio::mutable_buffers_1 Connection::buffer() {
            return asio::buffer(_buffer, _buffer.size());
        }

        bool Connection::processReceive(size_t bytes) {
            for (auto i = 0; i < bytes; i++) {
                if (_parser.advance(_buffer[i]) == Complete) {
	                auto &response = _parser.response();
                    auto cookies = response.headers("Set-Cookie");
                    for(auto cookie: cookies) {
                        _context.setCookie(cookie);
                    }
                    setResponse(response);
					_parser.reset();
                    return true;
                }
            }
            return false;
        }

        void Connection::setConnected() {
            _connected = true;
        }

	    std::string const & Connection::host() const
        {
	        return _host;
        }

	    int Connection::port() const
        {
	        return _port;
        }

	    std::vector<boost::asio::const_buffer> Connection::requestBuffers() const
        {
	        auto cookie = _context.cookieString(_request->url().path);
            _request->updateHeaders(cookie);
            return _request->toBuffers();
        }

	    ip::tcp::resolver::iterator Connection::endpoints()
        {
			ip::tcp::resolver::query query(_host, boost::lexical_cast<std::string>(_port));
			return _resolver.resolve(query);
		}

	    /*
	    void HttpConnection::send(std::shared_ptr<HttpRequest> &request) {

	    }

	    void HttpConnection::close() {

	    }
	     */


        SslConnection::SslConnection(HttpContext &context, std::string host, int port)
                : Connection(context, host, port == 0 ? 443 : port),
                  _socket(context.service(), context.ssl())
        { }

		void SslConnection::open(ConnectedHandler handler)
        {
			auto iterator = endpoints();

			_socket.set_verify_mode(ssl::verify_peer);

			_socket.set_verify_callback([this](bool preverified, ssl::verify_context &ctx) -> bool {
				return verifyCertificate(preverified, ctx);
			});

			asio::async_connect(_socket.lowest_layer(), iterator, 
				[this, handler](const error_code &ec, ip::tcp::resolver::iterator it) {
					if (ec) handler(ec);
					else handleConnect(handler);
				});
		}

	    void SslConnection::send(HttpRequestPtr request) {
            setRequest(request);

            asio::async_write(_socket, requestBuffers(), [this](const error_code &ec, size_t bytes) {
				if (ec) setError(ec);
                else handleWrite(bytes);
            });
        }

        void SslConnection::close() {
			if(_socket.lowest_layer().is_open()) {
				error_code ec;
				_socket.shutdown(ec);
			}
        }

	    Scheme SslConnection::scheme() const
        {
			return Https;
        }

		bool SslConnection::verifyCertificate(bool preverified, ssl::verify_context &context) {
			char subjectName[256];
			auto cert = X509_STORE_CTX_get_current_cert(context.native_handle());
			X509_NAME_oneline(X509_get_subject_name(cert), subjectName, 256);
			return preverified;
		}

		void SslConnection::handleConnect(std::function<void(const error_code &ec)> handler) {
			_socket.async_handshake(ssl::stream_base::client, [this, handler](const error_code &ec) {
				if (!ec) setConnected();
				handler(ec);
			});
		}

		void SslConnection::handleWrite(size_t bytesTransferred) {
			_socket.async_read_some(buffer(), [this](const error_code &ec, size_t bytes) {
				if (ec) setError(ec);
				else dataReceived(bytes);
			});
		}

		void SslConnection::dataReceived(size_t bytesTransferred) {
			if (bytesTransferred == 0) {
				std::cout << "hello";
			}
			if (processReceive(bytesTransferred)) return;
			_socket.async_read_some(buffer(), [this](const error_code &ec, size_t bytes) {
				if (ec) setError(ec);
				dataReceived(bytes);
			});
		}
	}
}
