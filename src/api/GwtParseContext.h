#ifndef QT_SECOND_GWTPARSECONTEXT_H
#define QT_SECOND_GWTPARSECONTEXT_H

#include <map>
#include <set>
#include <boost/shared_ptr.hpp>
#include "GwtRpcRequest.h"
#include "GwtReflectedType.h"
#include "GwtValue.h"

namespace vantagefx {
    namespace api {
        class GwtParseContext {
        public:
            typedef JsonVariantList::const_iterator const_iterator;

            GwtParseContext(StringList &&stringList, JsonVariantList &&data);

            GwtParseContext &operator>>(std::string &value);

            GwtParseContext &operator>>(int &value);

            GwtParseContext &operator>>(double &value);

            double popAsDouble();

			GwtValue popValue();

            int peekValueType();

            GwtParseContext &operator>>(int64_t &value);

            GwtParseContext &operator>>(boost::posix_time::ptime &value);

            bool eof() const { return _end == _it; }

            int maxWord() const { return _maxWord; }

            std::string peekType() const;

            void back(int count) { for(auto i = 0; i < count && _it != _data.end(); ++i) ++_it; }

            std::string word(int id);

            std::string str(int id);
			
            void print(std::ostream &stream, int count);

            int count() const;

        private:
            StringList _strings;
			JsonVariantList _data;
            const_iterator _it;
            const_iterator _end;
            int _maxWord = 1;
        };
    }
}

namespace std {
    template<typename Key, typename Value>
    std::ostream &operator<<(std::ostream &stream, const std::map<Key, Value> &item) {
        for (auto v: item) {
            stream << "<pair><key>" << v.first << "</key><value>" << v.second << "</value></pair>" << std::endl;
        }
        return stream;
    }

    template<typename Item>
    std::ostream &operator<<(std::ostream &stream, const std::vector<Item> &item) {
        for (auto v: item) {
            stream << "<item>" << v << "</item>" << std::endl;
        }
        return stream;
    }

    template<typename Item>
    std::ostream &operator<<(std::ostream &stream, const std::set<Item> &item) {
        for (auto v : item) {
            stream << "<item>" << v << "</item>" << std::endl;
        }
        return stream;
    }

    template<typename Item>
    std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<Item> &item) {
        stream << *item << std::endl;
        return stream;
    }
}


#endif //QT_SECOND_GWTPARSECONTEXT_H
