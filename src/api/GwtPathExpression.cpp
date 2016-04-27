//
// Created by AMakarenya on 04.04.2016.
//

#include "GwtPathExpression.h"
#include <boost/phoenix.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/make_fused.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/bind.hpp>
#include "GwtIterator.h"

namespace vantagefx {
    namespace api {
        namespace qi = boost::spirit::qi;

        namespace {
            template<typename T> struct make_tree_f {
                struct result {
                    typedef GwtPathTestPtr type;
                };
                GwtPathTestPtr operator()(std::vector<GwtPathTestPtr> &items) const
                {
                    GwtPathTestPtr root;
                    for(auto item :items) {
                        if (!root) root = item;
                        else root = std::make_shared<T>(root, item);
                    }
                    return root;
                }
            };

            template<typename T> struct make_shared_f {
                template<typename... A>
                struct result {
                    typedef std::shared_ptr<T> type;
                };

                template<typename... A>
                typename result<A...>::type operator()(A &... a) const
                {
                    return std::make_shared<T>(a ...);
                }
            };

            template<typename T> using make_tree_ = boost::phoenix::function<make_tree_f<T>>;

            template<typename T> using make_shared_ = boost::phoenix::function<make_shared_f<T>>;

        };

		template<typename Iterator, typename Skipper>
        struct PathParser : qi::grammar<Iterator, GwtPath(), Skipper> {

            PathParser() : PathParser::base_type(entry) {
				using qi::_1;
				using qi::_2;
				using qi::_val;
				using qi::lit;

                entry = -lit('/') >> *(part >> lit('/')) >> (part | func);
				part = (query | simple | any | many) >> !qi::lit('(');
                query = ('[' >> or_expr >> ']')[_val = boost::phoenix::construct<GwtPathExpression>(_1)];
				or_expr = (and_expr % "||")[_val = make_tree_<GwtPathTestOr>()(_1)];
                and_expr = (brackets % "&&")[_val = make_tree_<GwtPathTestAnd>()(_1)];
				brackets = ('(' >> or_expr >> ')') | eq_expr;
				eq_expr = ((entry | self) >> '=' >> (ref | ref_value))[_val = make_shared_<GwtPathTestEq>()(_1, _2)];
				self = lit('.') >> &lit('=')[_val = boost::phoenix::construct<GwtPath>()];
				value = (quoted | int_value | long_value | double_value);
				ref_value = value[_val = boost::phoenix::construct<GwtValueRef>(_1)];
				ref = ('{' >> qi::int_ >> '}')[_val = boost::phoenix::construct<GwtValueRef>(_1)];
				int_value = qi::int_ >> &lit(']');
				long_value = qi::long_long >> &lit(']');
				double_value = qi::double_ >> &lit(']');
				quoted = qi::lexeme['\'' >> *(escape | (qi::char_ - '\'')) >> '\''];
				func = func_name[_val = boost::phoenix::construct<GwtPathExpression>(_1)];
				func_name = string >> qi::char_('(') >> qi::char_(')');
				simple = string[_val = boost::phoenix::construct<GwtPathExpression>(_1)];
                any = lit('*') >> -lit('*')[_val = boost::phoenix::construct<GwtPathExpression>(false)];
				many = &lit('/')[_val = boost::phoenix::construct<GwtPathExpression>(true)];
                string = qi::lexeme[+(qi::alnum | qi::char_('-') | qi::char_('_'))];
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
			qi::rule<Iterator, GwtPathTestPtr(), Skipper> or_expr;
			qi::rule<Iterator, GwtPathTestPtr(), Skipper> and_expr;
			qi::rule<Iterator, GwtPathTestPtr(), Skipper> brackets;
			qi::rule<Iterator, GwtPathTestPtr(), Skipper> eq_expr;
			qi::rule<Iterator, GwtPath(), Skipper> self;
			qi::rule<Iterator, GwtPathExpression(), Skipper> func;
			qi::rule<Iterator, std::string(), Skipper> func_name;
			qi::rule<Iterator, GwtPathExpression(), Skipper> part;
            qi::rule<Iterator, GwtPathExpression(), Skipper> query;
            qi::rule<Iterator, GwtPathExpression(), Skipper> simple;
            qi::rule<Iterator, GwtPathExpression(), Skipper> any;
            qi::rule<Iterator, GwtPathExpression(), Skipper> many;
			qi::rule<Iterator, GwtValue(), Skipper> value;
			qi::rule<Iterator, GwtValueRef(), Skipper> ref_value;
			qi::rule<Iterator, int, Skipper> int_value;
			qi::rule<Iterator, int64_t, Skipper> long_value;
			qi::rule<Iterator, GwtValueRef(), Skipper> ref;
			qi::rule<Iterator, double, Skipper> double_value;
            qi::rule<Iterator, std::string(), Skipper> quoted;
            qi::rule<Iterator, std::string(), Skipper> string;
            qi::symbols<const char, const char> escape;
        };

	    GwtValueRef::GwtValueRef(int num)
			: _num(num)
	    {}

	    GwtValueRef::GwtValueRef()
			: _num(-1)
	    {}

	    GwtValueRef::GwtValueRef(const GwtValue& value)
			: _num(0),
			  _value(value)
	    {}

	    GwtValue GwtValueRef::operator()(const std::vector<GwtValue>& values) const
	    {
			if (_value.empty()) {
				if (_num < 0) return GwtValue();
				return values[_num];
			}
		    return _value;
	    }

	    GwtPathTestOr::GwtPathTestOr(const GwtPathTestPtr &left, const GwtPathTestPtr &right)
			: _left(left),
			_right(right)
		{}

	    bool GwtPathTestOr::match(const std::shared_ptr<const GwtObject> &object, const std::string &prefix, const std::vector<GwtValue> &values) const
	    {
			return _left->match(object, prefix, values) || _right->match(object, prefix, values);
		}

	    GwtPathTestAnd::GwtPathTestAnd(const GwtPathTestPtr &left, const GwtPathTestPtr &right)
			: _left(left),
			  _right(right)
		{}

	    bool GwtPathTestAnd::match(const std::shared_ptr<const GwtObject> &object, const std::string &prefix, const std::vector<GwtValue> &values) const
	    {
			return _left->match(object, prefix, values) && _right->match(object, prefix, values);
		}

	    GwtPathTestEq::GwtPathTestEq(GwtPath path, GwtValueRef value)
			: _path(path),
			  _value(value)
	    {}

	    bool GwtPathTestEq::match(const std::shared_ptr<const GwtObject> &object, const std::string &prefix, const std::vector<GwtValue> &values) const
	    {
			auto value = _value(values);
			for (GwtQueryIterator f(_path, object, values, prefix); f != GwtQueryIterator(); ++f) {
				if (f->value == value) return true;
			}
			return false;
		}

	    GwtPathExpression::GwtPathExpression(bool deep)
                : _deep(deep) { }

        GwtPathExpression::GwtPathExpression(const GwtPathTestPtr &test)
                : _deep(false),
			      _test(test)
		{ }

        GwtPathExpression::GwtPathExpression(std::string name)
                : _deep(false),
			      _name(name) { }

        GwtPath GwtPathExpression::parse(std::string path) {
            GwtPath result;
            PathParser<std::string::iterator, qi::space_type> parser;

	        auto it = path.begin();
            if (!qi::phrase_parse(it, path.end(), parser, qi::space, result) || it != path.end())
                throw std::runtime_error("bad path");

            return result;
        }
    }
}
