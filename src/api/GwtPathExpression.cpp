//
// Created by AMakarenya on 04.04.2016.
//

#include "GwtPathExpression.h"
#include <boost/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace vantagefx {
    namespace api {
        namespace qi = boost::spirit::qi;

        template<typename Iterator, typename Skipper>
        struct PathParser : qi::grammar<Iterator, GwtPath(), Skipper> {
            PathParser() : PathParser::base_type(entry) {
                entry = part % '/' >> -qi::lit('/');
                part = query | simple | many | any;
                query = ('[' >> (entry | self) >> '=' >> value >> ']')[qi::_val = boost::phoenix::construct<GwtPathExpression>(
                        qi::_1, qi::_2)];
				self = qi::lit('.') >> &qi::lit('=')[qi::_val = boost::phoenix::construct<GwtPath>()];
				value = quoted | string;
                quoted = qi::lexeme['\'' >> *(escape | (qi::char_ - '\'')) >> '\''];
                simple = string[qi::_val = boost::phoenix::construct<GwtPathExpression>(qi::_1)];
                any = qi::lit('*') >> -qi::lit('*')[qi::_val = boost::phoenix::construct<GwtPathExpression>(false)];
				many = qi::lit("**")[qi::_val = boost::phoenix::construct<GwtPathExpression>(true)];
                string = qi::lexeme[+qi::alnum];
                escape.add
                        ("\\\\", '\\')
                        ("\\'", '\'')
                        ("\\\"", '"')
                        ("\\r", '\r')
                        ("\\n", '\n')
                        ("\\0", '\0')
                        ("\\t", '\t');
            }

            qi::rule<Iterator, GwtPath(), Skipper> entry;
			qi::rule<Iterator, GwtPath(), Skipper> self;
			qi::rule<Iterator, GwtPathExpression(), Skipper> part;
            qi::rule<Iterator, GwtPathExpression(), Skipper> query;
            qi::rule<Iterator, GwtPathExpression(), Skipper> simple;
            qi::rule<Iterator, GwtPathExpression(), Skipper> any;
            qi::rule<Iterator, GwtPathExpression(), Skipper> many;
			qi::rule<Iterator, std::string(), Skipper> value;
            qi::rule<Iterator, std::string(), Skipper> quoted;
            qi::rule<Iterator, std::string(), Skipper> string;
            qi::symbols<const char, const char> escape;
        };

        GwtPathExpression::GwtPathExpression(bool deep)
                : _deep(deep) { }

        GwtPathExpression::GwtPathExpression(GwtPath filterPath, std::string filterValue)
                : _deep(false),
                  _filterPath(filterPath),
                  _filterValue(filterValue) { }

        GwtPathExpression::GwtPathExpression(std::string name)
                : _deep(false),
			      _name(name) { }

        GwtPath GwtPathExpression::parse(std::string path) {
            GwtPath result;
            PathParser<std::string::iterator, qi::space_type> parser;

            std::string::iterator it = path.begin();
            if (!qi::phrase_parse(it, path.end(), parser, qi::space, result) || it != path.end())
                throw std::runtime_error("bad path");

            return result;
        }
    }
}
