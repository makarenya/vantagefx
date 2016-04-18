//
// Created by alexx on 08.04.2016.
//

#ifndef VANTAGEFX_HTTPREQUEST_H
#define VANTAGEFX_HTTPREQUEST_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "Url.h"

namespace vantagefx {
    namespace http {

        namespace asio = boost::asio;
        namespace ssl = asio::ssl;
        namespace ip = asio::ip;
        using boost::system::error_code;

        class HttpRequest : public boost::noncopyable
        {
        public:

			HttpRequest();

	        explicit HttpRequest(std::string url, std::string method = "GET");

	        HttpRequest(HttpRequest &&rhs);

	        HttpRequest &operator=(HttpRequest &&rhs);

	        void updateHeaders(std::string cookie);

			std::vector<asio::const_buffer> toBuffers() const;

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

        private:
			std::string _method;
			Url _url;
			std::vector<std::pair<std::string, std::string>> _headers;
			std::string _content;
			bool _close;
		};
	}
}

#endif //VANTAGEFX_HTTPREQUEST_H
