//
// Created by alexx on 08.04.2016.
//

#ifndef VANTAGEFX_HTTPCONNECTION_H
#define VANTAGEFX_HTTPCONNECTION_H

#include <memory>
#include <future>
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "HttpResponseParser.h"


namespace vantagefx {
    namespace http {
        class HttpContext;
        class HttpRequest;

		typedef std::function<void(HttpResponse &&, const error_code &)> ReadyHandler;

		typedef std::function<void(const error_code &ec)> ConnectedHandler;

        class Connection : public std::enable_shared_from_this<Connection>
        {
        public:
	        explicit Connection(HttpContext &context, const std::string &scheme, const std::string &host, int port);

            virtual ~Connection() { }

            virtual void send(HttpRequest &&request, ReadyHandler &&handler);

            virtual void open() = 0;

            virtual void send() = 0;

            virtual void close() = 0;

			void closeConnection();

            virtual bool active();

			std::string scheme() const;

			std::string const &host() const;

			int port() const;

	        bool isHandled(HttpRequest &request);

        protected:

            asio::mutable_buffers_1 buffer();

            std::vector<boost::asio::const_buffer> requestBuffers();

			ip::tcp::resolver::query endpoints() const;

            bool processReceive(size_t bytes, bool proxy = false);

            bool handleError(const error_code &ec);

            void connect();

			bool disconnect();

	        const std::string &proxyGreeting() const;

        private:
			std::string _scheme;
			std::string _host;
			int _port;
            std::string _proxy;
            int _proxyPort;
            HttpContext &_context;
            HttpRequest _request;
			HttpResponse _response;
            HttpResponseParser _parser;
            std::array<char, 4096> _buffer;
			ip::tcp::resolver _resolver;
			std::atomic_bool _connected;
            bool _busy;
			std::string _proxyGreeting;
            ReadyHandler _handler;
        };

        /*
        class HttpConnection : public Connection
        {
        public:
            virtual void send(std::shared_ptr<HttpRequest> &request) override;

            virtual void close() override;
        private:
            ip::tcp::socket _socket;
        };
         */

        class SslConnection : public Connection
        {
        public:
	        SslConnection(HttpContext &context, const std::string &host, int port = 433);
			~SslConnection();

            void open() override;

            void send() override;

            void close() override;

			bool verifyCertificate(bool preverified, ssl::verify_context &context);

            void handleResolve(const error_code &ec, ip::tcp::resolver::iterator it);

            void handleConnect(const error_code &ec);

			void handleProxyWrite(const error_code &ec);

			void handleProxyRead(const error_code &ec, size_t bytesTransferred);

			void handleHandshake(const error_code &ec);

            void handleWrite(const error_code &ec);

            void dataReceived(const error_code &ec, size_t bytesTransferred);

        private:
            ssl::stream<ip::tcp::socket> _socket;
            ip::tcp::resolver _resolver;
        };
    }
}



#endif //VANTAGEFX_HTTPCONNECTION_H
