//
// Created by alexx on 21.03.2016.
//

#include "RequestParser.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/locale/utf.hpp>

namespace vantagefx {
    namespace api {

        namespace qi = boost::spirit::qi;

        template<typename Iterator>
        struct request_parser : public qi::grammar<Iterator, JsonVariantList()> {
            request_parser() : request_parser::base_type(file) {
                file = +(item >> '|');
                item = int_value | double_value | string_value;
                int_value = qi::int_ >> !qi::alnum;
                double_value = qi::double_ >> !qi::alnum;
                string_value = *(escape_char | (qi::char_ - '|'));
                escape_char.add("\\\\", '\\')("\\!", '|')("\\0", '\0');
            }

            qi::rule<Iterator, JsonVariant()> item;
            qi::rule<Iterator, JsonVariantList()> file;
            qi::rule<Iterator, int()> int_value;
            qi::rule<Iterator, double()> double_value;
            qi::rule<Iterator, std::string()> string_value;
            qi::symbols<const char, const char> escape_char;
        };

        JsonVariantList ParseRequest(std::string request) {
            auto it = request.cbegin();
            auto end = request.cend();
            request_parser<std::string::const_iterator> parser;
            JsonVariantList result;
            if (qi::parse(it, end, parser, result) && it == end) {
                return result;
            }
            return JsonVariantList();
        }
    }
}
