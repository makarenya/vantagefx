#include <boost/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>
#include "GwtValue.h"
#include "GwtResponseParser.h"

BOOST_FUSION_ADAPT_STRUCT(
        vantagefx::api::GwtResponseData,
        (vantagefx::api::JsonVariantList, data)
        (vantagefx::api::StringList, strings)
        (int, flags)
        (int, version)
);


namespace vantagefx {
    namespace api {

        namespace qi = boost::spirit::qi;

        template<typename Iterator>
        struct ResponseParser : qi::grammar<Iterator, GwtResponseData()> {
            ResponseParser()
                    : ResponseParser::base_type(response) {
                using qi::lit;
                using qi::alnum;
                using qi::double_;
                using qi::int_;
                using qi::bool_;
                using qi::lexeme;
                using qi::char_;
                using qi::hex;

                unesc_char.add
                        ("\\0", '\0')
                        ("\\a", '\a')
                        ("\\b", '\b')
                        ("\\f", '\f')
                        ("\\n", '\n')
                        ("\\r", '\r')
                        ("\\t", '\t')
                        ("\\v", '\v')
                        ("\\\\", '\\')
                        ("\\\'", '\'')
                        ("\\\"", '\"');

                string_item = '"' >> *(unesc_char | ("\\x" >> hex_char) | (char_ - '"')) >> '"';
                string_list = string_item % ',';

                json_string = '\'' >> *(char_ - '\'') >> '\'';
                int_rule = int_ >> !double_;

                json_item = int_rule | double_ | json_string;
                data_list = +(json_item >> (',' | qi::lit("].concat([")));
				response = "//OK[" >> data_list >> '[' >> string_list >> ']' >> 
					',' >> int_ >> ',' >> int_ >> ']' >> -qi::lit(')');
            }

            qi::rule<Iterator, GwtResponseData()> response;
            qi::rule<Iterator, JsonVariantList()> data_list;
            qi::rule<Iterator, JsonVariant()> json_item;
            qi::rule<Iterator, std::string()> json_string;

            qi::rule<Iterator, StringList()> string_list;
            qi::rule<Iterator, std::string()> string_item;
            qi::rule<Iterator, int> int_rule;
            qi::uint_parser<char, 16, 2, 2> hex_char;
            qi::symbols<char const, char const> unesc_char;
        };

		GwtParser makeResponseParser(std::string response, GwtBundle &bundle) {
            ResponseParser<std::string::const_iterator> parser;
			GwtResponseData data;

            auto it = response.cbegin();
            auto end = response.cend();

            if (parse(it, end, parser, data) && it == end) {
				return GwtParser(std::move(data.strings), std::move(data.data), bundle);
            }
			throw std::runtime_error("GWT parse fails");
        }
    }
}
