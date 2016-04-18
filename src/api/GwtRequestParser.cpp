//
// Created by AMakarenya on 30.03.2016.
//

#include "GwtRequestParser.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/tuple/tuple.hpp>


namespace qi = boost::spirit::qi;
namespace pnx = boost::phoenix;


namespace vantagefx {
    namespace api {

        GwtParser makeRequestParser(std::string content, GwtBundle &bundle)
        {
            GwtResponseData result;
            auto it = content.cbegin();
            auto end = content.cend();
            if (!qi::parse(it, end, qi::int_ >> '|', result.version)) return GwtParser();
            if (!qi::parse(it, end, qi::int_ >> '|', result.flags)) return GwtParser();
            int count;
            if (!qi::parse(it, end, qi::int_ >> '|', count)) return GwtParser();
            qi::symbols<const char, const char> symbols;
            symbols.add("\\\\", '\\')("\\!", '!')("\\0", '\0');
            for(auto i = 0; i < count; i++) {
                std::string value;
                if (!qi::parse(it, end, +(symbols | (qi::char_ - '|')) >> '|', value)) return GwtParser();
                result.strings.push_back(value);
            }
            qi::rule<std::string::const_iterator, JsonVariant()> item;
            item = (qi::int_ >> !(qi::char_ - '|')) | (qi::double_ >> !(qi::char_ - '|')) | +(qi::char_ - '|');
            if (!qi::parse(it, end, +(item >> '|'), result.data)) return GwtParser();
            std::reverse(result.data.begin(), result.data.end());
			return GwtParser(result.strings, result.data, bundle);
        }
    }
}
