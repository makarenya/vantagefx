//
// Created by alexx on 08.04.2016.
//


#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "HttpRequest.h"

namespace vantagefx {
    namespace http {

		namespace strings {
			const std::string space = " ";
			const std::string http = "HTTP/1.1";
			const std::string crlf = "\r\n";
			const std::string nameValueSeparator = ": ";
			const std::string host = "Host";
			const std::string accept = "Accept";
			const std::string userAgent = "User-Agent";
			const std::string contentLength = "Content-Length";
			const std::string cookie = "Cookie";
			const std::string acceptValue = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
			const std::string userAgentValue = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.110 Safari/537.36";
			const std::string connection = "Connection";
			const std::string close = "Close";
			const std::string keepAlive = "Keep-Alive";
		}

	    HttpRequest::HttpRequest()
				: _method(),
				  _url(),
				  _close(false)
		{}

		HttpRequest::HttpRequest(std::string url, std::string method)
                : _method(method),
			      _url(url),
			      _close(false)
        {}

	    HttpRequest::HttpRequest(HttpRequest &&rhs)
	    {
		    _method = std::move(rhs._method);
		    _url = std::move(rhs._url);
		    _headers = std::move(rhs._headers);
		    _content = std::move(rhs._content);
		    _close = rhs._close;
	    }

	    HttpRequest & HttpRequest::operator=(HttpRequest &&rhs)
	    {
		    if (&rhs != this) {
			    _method = std::move(rhs._method);
			    _url = std::move(rhs._url);
			    _headers = std::move(rhs._headers);
			    _content = std::move(rhs._content);
			    _close = rhs._close;
		    }
		    return *this;
	    }

	    void HttpRequest::updateHeaders(std::string cookie)
		{
			if (!hasHeader(strings::host)) {
				addHeader(strings::host, _url.host());
			}
			if (!hasHeader(strings::accept)) {
				addHeader(strings::accept, strings::acceptValue);
			}
			if (!hasHeader(strings::userAgent)) {
				addHeader(strings::userAgent, strings::userAgentValue);
			}
			if (!hasHeader(strings::contentLength) && _content.length() > 0) {
				addHeader(strings::contentLength, boost::lexical_cast<std::string>(_content.length()));
			}
			if (!hasHeader(strings::cookie) && !cookie.empty()) {
				addHeader(strings::cookie, cookie);
			}
			if (!hasHeader(strings::connection)) {
				addHeader(strings::connection, _close ? strings::close : strings::keepAlive);
			}
		}

		std::vector<asio::const_buffer> HttpRequest::toBuffers() const
		{
			std::vector<asio::const_buffer> buffers;
			buffers.push_back(asio::buffer(_method));
			buffers.push_back(asio::buffer(strings::space));
			buffers.push_back(asio::buffer(_url.uri()));
			buffers.push_back(asio::buffer(strings::space));
			buffers.push_back(asio::buffer(strings::http));
			buffers.push_back(asio::buffer(strings::crlf));

			std::vector<std::pair<std::string, std::string>> headers(_headers.begin(), _headers.end());

			for (auto i = 0; i < _headers.size(); ++i)
			{
				auto &header = _headers[i];
				buffers.push_back(asio::buffer(header.first));
				buffers.push_back(asio::buffer(strings::nameValueSeparator));
				buffers.push_back(asio::buffer(header.second));
				buffers.push_back(asio::buffer(strings::crlf));
			}
			buffers.push_back(asio::buffer(strings::crlf));
			buffers.push_back(asio::buffer(_content));

			return buffers;
		}

	    void HttpRequest::setMethod(std::string method)
        {
			_method = method;
        }

	    std::string const & HttpRequest::method() const
        {
			return _method;
        }

	    void HttpRequest::setUrl(const Url &url)
        {
			_url = url;
        }

	    Url const &HttpRequest::url() const
        {
			return _url;
        }

	    void HttpRequest::addHeader(std::string key, std::string value)
        {
			_headers.push_back(std::make_pair(key, value));
        }

	    void HttpRequest::setContent(std::string content)
		{
			_content = content;
		}

	    std::string const & HttpRequest::content() const
		{
			return _content;
		}

	    void HttpRequest::setClose(bool close)
		{
			_close = close;
		}

	    bool HttpRequest::close() const
		{
			return _close;
		}

	    std::string HttpRequest::header(std::string key) const
        {
	        for(auto pair: _headers) {
				if (boost::iequals(pair.first, key)) return pair.second;
	        }
			return std::string();
        }

	    bool HttpRequest::hasHeader(std::string key) const
		{
			for (auto pair : _headers) {
				if (boost::iequals(pair.first, key)) return true;
			}
			return false;
		}
    }
}
