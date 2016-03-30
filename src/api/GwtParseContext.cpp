#include "GwtParseContext.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace vantagefx {
    namespace api {
        GwtParseContext::GwtParseContext(StringList &stringList, JsonVariantList &data) {
            _strings = stringList;
            _it = data.cend();
            _end = data.cbegin();
        }

        GwtParseContext &GwtParseContext::operator>>(std::string &value) {
            auto id = boost::get<int>(*--_it);
            value = word(id);
            return *this;
        }

        GwtParseContext &GwtParseContext::operator>>(int &value) {
            value = boost::get<int>(*--_it);
            return *this;
        }

        GwtParseContext &GwtParseContext::operator>>(double &value) {
            value = boost::get<double>(*--_it);
            return *this;
        }

        GwtParseContext &GwtParseContext::operator>>(bool &value) {
            value = boost::get<bool>(*--_it);
            return *this;
        }

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
            throw new std::runtime_error("invalid base64 symbol");
        }

        GwtParseContext &GwtParseContext::operator>>(int64_t &value) {
            auto val = boost::get<std::string>(*--_it);
            int64_t result = 0;
            for (auto i = 0; i < val.length(); i++) {
                result = result << 6 | base64Value(val[i]);
            }
            value = result;
            return *this;
        }

        GwtParseContext &GwtParseContext::operator>>(boost::posix_time::ptime &value) {
            using boost::posix_time::ptime;
            using boost::posix_time::milliseconds;
            namespace gregorian = boost::gregorian;
            int64_t t;
            *this >> t;
            ptime start(gregorian::date(1970, 1, 1));
            value = start + milliseconds(static_cast<long>(t));
            return *this;
        }

        std::string GwtParseContext::typeName(int id) {
            using boost::algorithm::split;
            using boost::algorithm::is_any_of;

            auto name = word(id);
            std::vector<std::string> parts;
            split(parts, name, is_any_of("/"));
            return parts[0];
        }

        struct print_visitor : boost::static_visitor<std::ostream &> {
            print_visitor(std::ostream &stream, GwtParseContext &ctx)
                    : _ctx(ctx),
                      _stream(stream) { }

            std::ostream &operator()(int value) const {
                if (value > 0 && value <= _ctx.maxWord()) {
                    return _stream << value << " [" << _ctx.word(value) << "]" << std::endl;
                }
                return _stream << value << std::endl;
            }

            template<typename T>
            std::ostream &operator()(T value) const {
                return _stream << value << std::endl;
            }

        private:
            GwtParseContext &_ctx;
            std::ostream &_stream;
        };

        std::string GwtParseContext::peekType() const {
            return _it->type().name();
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
                boost::apply_visitor(print_visitor(stream, *this), *--_it);
            }
        }

        int GwtParseContext::count() const {
            return static_cast<int>(std::distance(_end, _it));
        }
    }
}
