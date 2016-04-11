//
// Created by alexx on 08.04.2016.
//

#ifdef _MSC_VER
#pragma warning(disable : 4503)
#pragma warning(push)
#pragma warning(disable: 4348) // possible loss of data
#endif

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted.hpp>
#include "HttpRequest.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif



BOOST_FUSION_ADAPT_STRUCT(
	vantagefx::http::Url,
	(vantagefx::http::Scheme, scheme)
	(std::string, host)
	(int, port)
	(std::string, path)
	(std::string, hash)
)

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

        namespace parser {
            namespace qi = boost::spirit::qi;

			template<typename Iterator>
            struct UrlParser : qi::grammar<Iterator, Url()> {
	            UrlParser();

	            qi::rule<Iterator, Url()> entry;
                qi::rule<Iterator, std::string()> string;
				qi::rule<Iterator, std::string()> domain;
				qi::rule<Iterator, char()> escaped;
                qi::symbols<const char, Scheme> scheme;
            };

	        template <typename Iterator>
	        UrlParser<Iterator>::UrlParser()
				: UrlParser::base_type(entry)
	        {
		        entry = scheme >> "://" >> domain >> -(qi::lit(':') >> qi::int_) >> -(qi::char_('/') >> string) >>
			        -(qi::char_('#') >> string);
				string = +(escaped | qi::alnum | qi::char_("$_.+!*'(),/-"));
		        domain = +(escaped | qi::alnum | qi::char_("$_.+!*'(),-"));
		        escaped = qi::lit('%') >> qi::int_;
		        scheme.add("https", Https)("http", Http);
	        }

	        Url parse(std::string url)
            {
                Url result;
                UrlParser<std::string::iterator> p;
                qi::parse(url.begin(), url.end(), p, result);
				result.uri = url;
                return result;
            }
		}

		std::string Url::serverUrl() const
		{
			if (port == 0) return schemeName() + host;
			return schemeName() + host + ":" + boost::lexical_cast<std::string>(port);
		}

		std::string Url::schemeName() const
		{
			switch(scheme) {
				case Https: return "https://";
				default: return "http://";
			}
		}

		HttpRequest::HttpRequest(std::string url, std::string method)
                : _method(method),
			      _url(parser::parse(url)),
			      _close(false)
        {}

	    void HttpRequest::updateHeaders(std::string cookie)
		{
			if (!hasHeader(strings::host)) {
				addHeader(strings::host, _url.host);
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
			buffers.push_back(asio::buffer(_url.uri));
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

	    HttpResponse const & HttpRequest::response() const
        {
			return _response;
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

	    void HttpRequest::setHandler(std::function<void(std::shared_ptr<HttpRequest> request)> handler)
		{
			_handler = handler;
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

        void HttpRequest::setResponse(HttpResponse &response) {
            _response = response;
			_handler(shared_from_this());
		}

        void HttpRequest::setError(const error_code &ec) {
            _error = ec;
			_handler(shared_from_this());
        }

        error_code const &HttpRequest::error() const {
            return _error;
        }
    }
}
