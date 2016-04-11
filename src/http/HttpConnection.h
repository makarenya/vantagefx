//
// Created by alexx on 08.04.2016.
//

#ifndef VANTAGEFX_HTTPCONNECTION_H
#define VANTAGEFX_HTTPCONNECTION_H

#include <memory>
#include "HttpResponse.h"
#include "HttpRequest.h"


namespace vantagefx {
    namespace http {
        class HttpContext;
        class HttpRequest;
		class Connection;

		typedef std::shared_ptr<Connection> ConnectionPtr;

		typedef std::function<void(const error_code &ec)> ConnectedHandler;

        class Connection : public std::enable_shared_from_this<Connection>
        {
        public:
	        explicit Connection(HttpContext &context, std::string host, int port);
            virtual ~Connection() { }
			virtual void open(ConnectedHandler handler) = 0;
            virtual void send(HttpRequestPtr request) = 0;
            virtual void close() = 0;
			void closeConnection();
            virtual bool active();
			virtual Scheme scheme() const = 0;
			std::string const &host() const;
			int port() const;

        protected:

            void setRequest(HttpRequestPtr &request);

            void setResponse(HttpResponse &response);

            void setError(const error_code &ec);

            asio::mutable_buffers_1 buffer();

            std::vector<boost::asio::const_buffer> requestBuffers() const;

			ip::tcp::resolver::iterator endpoints();

            bool processReceive(size_t bytes);

            void setConnected();

        private:
			std::string _host;
			int _port;
            HttpContext &_context;
            HttpRequestPtr _request;
            HttpResponseParser _parser;
            std::array<char, 4096> _buffer;
			ip::tcp::resolver _resolver;
			bool _connected;
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
	        SslConnection(HttpContext &context, std::string host, int port = 433);

			void open(ConnectedHandler handler) override;

            void send(HttpRequestPtr request) override;

            void close() override;

			Scheme scheme() const override;
			
			bool verifyCertificate(bool preverified, ssl::verify_context &context);

            void handleConnect(std::function<void(const error_code &ec)> handler);

            void handleWrite(size_t bytesTransferred);

            void dataReceived(size_t bytesTransferred);

        private:
            ssl::stream<ip::tcp::socket> _socket;
        };
    }
}



#endif //VANTAGEFX_HTTPCONNECTION_H
