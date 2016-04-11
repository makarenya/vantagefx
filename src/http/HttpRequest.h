//
// Created by alexx on 08.04.2016.
//

#ifndef VANTAGEFX_HTTPREQUEST_H
#define VANTAGEFX_HTTPREQUEST_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "HttpResponseParser.h"

namespace vantagefx {
    namespace http {

        namespace asio = boost::asio;
        namespace ssl = asio::ssl;
        namespace ip = asio::ip;
        using boost::system::error_code;

        enum Scheme {
            Http,
            Https
        };

        struct Url {
            Scheme scheme;
            std::string host;
            int port = 0;
            std::string path;
            std::string hash;
			std::string uri;

            std::string schemeName() const;
            std::string serverUrl() const;

			operator std::string() const
			{
				return uri;
			}
        };

        class HttpRequest : public std::enable_shared_from_this<HttpRequest>
        {
        public:

	        explicit HttpRequest(std::string url, std::string method = "GET");

			void updateHeaders(std::string cookie);

			std::vector<asio::const_buffer> toBuffers() const;

            void setResponse(HttpResponse &response);
            void setError(const error_code &ec);
            error_code const &error() const;
			HttpResponse const &response() const;

			void setMethod(std::string method);
			std::string const &method() const;

			void setUrl(const Url &url);
			Url const &url() const;

			void addHeader(std::string key, std::string value);
			std::string header(std::string key) const;
			bool hasHeader(std::string key) const;

			void setContent(std::string content);
			std::string const &content() const;

	        void setClose(bool close);
	        bool close() const;

			void setHandler(std::function<void(std::shared_ptr<HttpRequest>)> handler);

        private:
            error_code _error;
			std::string _method;
			Url _url;
			std::vector<std::pair<std::string, std::string>> _headers;
			std::string _content;
            HttpResponse _response;
			std::function<void(std::shared_ptr<HttpRequest> request)> _handler;
			bool _close;
		};

		typedef std::shared_ptr<HttpRequest> HttpRequestPtr;
	}
}


#endif //VANTAGEFX_HTTPREQUEST_H
