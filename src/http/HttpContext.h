//
// Created by alexx on 08.04.2016.
//

#ifndef VANTAGEFX_HTTPCLIENT_H
#define VANTAGEFX_HTTPCLIENT_H


#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <set>
#include <future>
#include "HttpConnection.h"

namespace vantagefx {
    namespace http {

        namespace asio = boost::asio;
        namespace ssl = asio::ssl;
        namespace ip = asio::ip;
        using boost::system::error_code;

        class HttpRequest;

		class HttpResponse;

        class HttpContext
		{
        public:
			typedef std::map<std::string, std::string> CookieParameters;
			typedef std::pair<std::string, CookieParameters> Cookie;
			typedef std::map<std::string, Cookie> CookieCollection;
            typedef std::shared_ptr<Connection> ConnectionPtr;

            HttpContext(boost::asio::io_service &io_service, boost::asio::ssl::context &context);

			void setCookie(std::string cookie);
			std::string cookieString(std::string path) const;
	        void closeConnection(ConnectionPtr connection);

            void send(HttpRequest &&request, ReadyHandler &&handler);

			std::future<HttpResponse> send(HttpRequest &&request);

	        boost::asio::io_service &service() const;

	        boost::asio::ssl::context &ssl() const;

        private:

			CookieCollection _cookies;
            std::set<ConnectionPtr> _connections;
            boost::asio::io_service &_io_service;
            boost::asio::ssl::context &_context;
        };
    }
}


#endif //VANTAGEFX_HTTPCLIENT_H
