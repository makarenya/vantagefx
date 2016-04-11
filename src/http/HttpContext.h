//
// Created by alexx on 08.04.2016.
//

#ifndef VANTAGEFX_HTTPCLIENT_H
#define VANTAGEFX_HTTPCLIENT_H


#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>

namespace vantagefx {
    namespace http {

        namespace asio = boost::asio;
        namespace ssl = asio::ssl;
        namespace ip = asio::ip;
        using boost::system::error_code;

        class HttpRequest;

		class Connection;

        class HttpContext
		{
        public:
            HttpContext(boost::asio::io_service &io_service, boost::asio::ssl::context &context);

			void setCookie(std::string cookie);
			std::string cookieString(std::string path) const;
	        void closeConnection(std::shared_ptr<Connection> connection);
	        typedef std::map<std::string, std::string> CookieParameters;
			typedef std::pair<std::string, CookieParameters> Cookie;
			typedef std::map<std::string, Cookie> CookieCollection;

            void send(std::shared_ptr<HttpRequest> req);
			void get(std::string url, std::function<void(std::shared_ptr<HttpRequest> request)> handler);

	        boost::asio::io_service &service() const;

	        boost::asio::ssl::context &ssl() const;

        private:

			CookieCollection _cookies;
            std::vector<std::shared_ptr<Connection>> _connections;
            boost::asio::io_service &_io_service;
            boost::asio::ssl::context &_context;
        };
    }
}


#endif //VANTAGEFX_HTTPCLIENT_H
