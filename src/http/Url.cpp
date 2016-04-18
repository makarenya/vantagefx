//
// Created by alexx on 17.04.2016.
//

#include "Url.h"

#ifdef _MSC_VER
#pragma warning(disable : 4503)
#pragma warning(push)
#pragma warning(disable: 4348) // possible loss of data
#endif

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/tuple.hpp>
#include <string>

#ifdef _MSC_VER
#pragma warning(pop)
#endif


namespace vantagefx {
    namespace http {
        namespace parser {
            namespace qi = boost::spirit::qi;
            using boost::optional;

            typedef std::tuple<optional<std::string>, std::string, optional<int>,
                    optional<std::string>, optional<std::string>> RawUrl;

            template<typename Iterator>
            struct UrlParser : qi::grammar<Iterator, RawUrl()> {
                UrlParser();

                qi::rule<Iterator, RawUrl()> entry;
                qi::rule<Iterator, std::string()> string;
                qi::rule<Iterator, std::string()> domain;
                qi::rule<Iterator, char()> escaped;
                qi::rule<Iterator, std::string()> scheme;
            };

            template <typename Iterator>
            UrlParser<Iterator>::UrlParser()
                    : UrlParser::base_type(entry)
            {
                entry = -(scheme >> "://") >> domain >> -(qi::lit(':') >> qi::int_) >>
                        -(qi::char_('/') >> string) >> -(qi::char_('#') >> string);

                string = +(escaped | qi::alnum | qi::char_("$_.+!*'(),/-"));
                domain = +(escaped | qi::alnum | qi::char_("$_.+!*'(),-"));
                escaped = qi::lit('%') >> qi::int_;
                scheme = +(qi::alnum);
            }
        }

        Url::Url()
                : _port(0)
        {}

        Url::Url(std::string url)
                : _port(0)
        {
            parser::RawUrl raw;
            parser::UrlParser<std::string::iterator> urlParser;
            boost::spirit::qi::parse(url.begin(), url.end(), urlParser, raw);

            auto scheme = std::get<0>(raw);
            if (scheme) _scheme = *scheme;

            _host = std::get<1>(raw);

            auto port = std::get<2>(raw);
            if (port) _port = *port;

            auto path = std::get<3>(raw);
            if (path) _path = *path;

            auto hash = std::get<4>(raw);
            if (hash) _hash = *hash;

            _uri = url;
        }

        std::string Url::serverUrl() const
        {
            if (port() == 0) return scheme() + "://" + host();
            return scheme() + "://" + host() + ":" + boost::lexical_cast<std::string>(port());
        }

        Url::operator std::string() const
        {
            return _uri;
        }
    }
}