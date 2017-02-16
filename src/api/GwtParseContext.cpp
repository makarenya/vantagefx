#include "GwtParseContext.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace vantagefx {
    namespace api {
		namespace base64 {
			inline uint64_t base64Value(char c) {
				if (c >= 'A' && c <= 'Z') {
					return c - 'A';
				}
				if (c >= 'a' && c <= 'z') {
					return c - 'a' + 26;
				}
				if (c >= '0' && c <= '9') {
					return c - '0' + 52;
				}
				if (c == '$') return 62;
				if (c == '_') return 63;
				if (c == '=') return 0;
				throw std::runtime_error("invalid base64 symbol");
			}

			int64_t base64Decode(std::string val)
			{
				int64_t result = 0;
				for (auto i = 0; i < val.length(); i++) {
					result = result << 6 | base64Value(val[i]);
				}
				return result;
			}
		}

        namespace visitors {
            struct double_visitor : public boost::static_visitor<double> {
                double operator()(int value) const {
                    return value;
                }
                double operator()(double value) const {
                    return value;
                }
                double operator()(int64_t value) const {
                    return static_cast<double>(value);
                }
                template<typename T>
                double operator()(T value) const {
                    return 0;
                }
            };

			struct value_visitor : public boost::static_visitor<GwtValue>
			{
				GwtValue operator()(int value) const {
					return GwtValue(value);
				}
				GwtValue operator()(double value) const {
					return GwtValue(value);
				}
				GwtValue operator()(std::string value) const {
					return GwtValue(base64::base64Decode(value));
				}
			};
		}

	    GwtParseContext::GwtParseContext(StringList &&stringList, JsonVariantList &&data) {
            _strings = std::move(stringList);
			for(auto it = data.begin(); it != data.end(); ++it) {
				_data.push_back(std::make_pair(*it, ""));
			}
            _it = _data.end();
            _end = _data.begin();
        }

        void GwtParseContext::pop(std::string &value, const std::string &what) {
            auto id = boost::get<int>((--_it)->first);
			_it->second = what;
            value = word(id);
        }

		void GwtParseContext::pop(int &value, const std::string &what) {
            value = boost::get<int>((--_it)->first);
			_it->second = what;
		}

		void GwtParseContext::pop(double &value, const std::string &what) {
            value = boost::get<double>((--_it)->first);
			_it->second = what;
		}

		void GwtParseContext::pop(int64_t &value, const std::string &what) {
			auto val = boost::get<std::string>((--_it)->first);
			value = base64::base64Decode(val);
			_it->second = what;
		}

		void GwtParseContext::pop(boost::posix_time::ptime &value, const std::string &what) {
			using boost::posix_time::ptime;
			using boost::posix_time::milliseconds;
			namespace gregorian = boost::gregorian;
			int64_t t;
			pop(t, what);
			ptime start(gregorian::date(1970, 1, 1));
			value = start + milliseconds(static_cast<long>(t));
		}
		
    	double GwtParseContext::popAsDouble(const std::string &what) {
            auto value = boost::apply_visitor(visitors::double_visitor(), (--_it)->first);
			_it->second = what;
			return value;
		}

	    GwtValue GwtParseContext::popValue(const std::string &what)
        {
			auto value = boost::apply_visitor(visitors::value_visitor(), (--_it)->first);
			_it->second = what;
			return value;
		}

        int GwtParseContext::peekValueType() const
        {
            if (_strings.size() > 0) return -1;
            if (eof()) return -1;
            return (_it - 1)->first.which();
        }

        struct print_visitor : boost::static_visitor<std::ostream &> {
            print_visitor(std::ostream &stream, GwtParseContext &ctx)
                    : _ctx(ctx),
                      _stream(stream) { }

            std::ostream &operator()(int value) const {
                if (value > 0 && value <= _ctx.maxWord()) {
                    return _stream << value << " [" << _ctx.word(value) << "]";
                }
                return _stream << value;
            }

			std::ostream &operator()(double value) const {
				return _stream << std::scientific << value;
			}

            template<typename T>
            std::ostream &operator()(T value) const {
                return _stream << value;
            }

        private:
            GwtParseContext &_ctx;
            std::ostream &_stream;
        };

        std::string GwtParseContext::peekType() const {
            switch (_it->first.which()) {
                case 0:
                    return "int";
                case 1:
                    return "double";
                default:
                    return "string";
            }
        }

        std::string GwtParseContext::word(int id) {
            if (id == 0) return std::string();
            if (id > _maxWord) {
                throw std::runtime_error(
                        "can't read word with index " + boost::lexical_cast<std::string>(id) +
                        ", while max word is: " + boost::lexical_cast<std::string>(_maxWord));
            }
            _maxWord = std::max(_maxWord, id + 1);
            return _strings[id - 1];
        }

        std::string GwtParseContext::str(int id) {
            if (id <= 0 || id > _maxWord) return std::string();
            return _strings[id - 1];
        }

        void GwtParseContext::print(std::ostream &stream, int count) {
            for (auto i = 0; i < count; i++) {
                if (_it == _end) return;
	            --_it;
                boost::apply_visitor(print_visitor(stream, *this), _it->first);
				if (!_it->second.empty())
				{
					stream << "\t\t#" << _it->second;
				}
				stream << std::endl;
            }
        }

        int GwtParseContext::count() const {
            return static_cast<int>(std::distance(_end, _it));
        }
    }
}
