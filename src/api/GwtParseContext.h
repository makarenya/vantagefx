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
			typedef std::vector<std::pair<JsonVariant, std::string>> ItemsList;
            typedef ItemsList::iterator iterator;

            GwtParseContext(StringList &&stringList, JsonVariantList &&data);

			void pop(std::string &value, const std::string &what);

        	void pop(int &value, const std::string &what);

        	void pop(double &value, const std::string &what);

			void pop(int64_t &value, const std::string &what);

			void pop(boost::posix_time::ptime &value, const std::string &what);

        	double popAsDouble(const std::string &what);

			GwtValue popValue(const std::string &what);

            int peekValueType() const;


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
			ItemsList _data;
            iterator _it;
            iterator _end;
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
