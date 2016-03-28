#ifndef QT_SECOND_GWTPARSECONTEXT_H
#define QT_SECOND_GWTPARSECONTEXT_H

#include <map>
#include <set>
#include <boost/shared_ptr.hpp>
#include "GwtRpcRequest.h"
#include "GwtReflectedType.h"
#include "GwtObjectWrapper.h"

namespace vantagefx
{
	namespace api
	{
		class GwtParseContext
		{
		public:
			typedef JsonVariantList::const_iterator const_iterator;

			GwtParseContext(StringList& stringList, JsonVariantList& data);

			GwtParseContext& operator>>(std::string& value);
			GwtParseContext& operator>>(int& value);
			GwtParseContext& operator>>(double& value);
			GwtParseContext& operator>>(bool& value);
            GwtParseContext &operator>>(int64_t &value);
			GwtParseContext& operator>>(boost::posix_time::ptime& value);
			template<typename T>
			GwtParseContext& operator>>(const CheckedType<T>& tc);
            template<typename T>
            GwtParseContext& operator>>(const CheckedType<std::shared_ptr<T>>& tc);

			bool eof() const { return _end == _it; }
			int maxWord() const { return _maxWord; }
			std::string peekType() const;

			std::string word(int id);
			std::string str(int id);
			std::string typeName(int id);
			void print(std::ostream& stream, int count);
			int count() const;
		private:
			std::vector<std::shared_ptr<IGwtObjectWrapper>> _created;
			StringList _strings;
			const_iterator _it;
			const_iterator _end;
			int _maxWord = 1;
		};

		template<typename T>
		inline GwtParseContext& GwtParseContext::operator>>(const CheckedType<T>& tc)
		{
			int typeId;
			*this >> typeId;
			if (typeId > 0)
			{
                GwtReflectedType<T>::check(typeName(typeId));
				*this >> tc.value();
				_created.push_back(wrap(tc.value(), typeName(typeId)));
			}
			else
			{
				auto wrapper = _created[~typeId];
                GwtReflectedType<T>::check(wrapper->name());
                auto typeWrapper = static_cast<GwtObjectWrapper<T>*>(wrapper.get());
                tc.value() = *typeWrapper->value();
                _created.push_back(wrapper);
			}
			return *this;
		};

        template<typename T>
        inline GwtParseContext& GwtParseContext::operator>>(const CheckedType<std::shared_ptr<T>>& tc)
        {
			int typeId;
			*this >> typeId;
			if (typeId == 0)
			{
				tc.value() = nullptr;
				_created.push_back(wrap(nullptr));
			}
			else if (typeId > 0)
			{
                GwtReflectedType<T>::check(typeName(typeId));
                std::shared_ptr<T> item = GwtReflectedType<T>::create(typeId, *this);
				tc.value() = item;
				_created.push_back(wrap(item, typeName(typeId)));
			}
			else
			{
				auto wrapper = _created[~typeId];
                GwtReflectedType<T>::check(wrapper->name());
                auto typeWrapper = static_cast<GwtObjectWrapper<T>*>(wrapper.get());
				tc.value() = typeWrapper->value();
                _created.push_back(wrapper);
			}
			return *this;
		};

        template<typename Key, typename Value>
        GwtParseContext &operator>>(GwtParseContext &ctx, std::map<Key, Value>& result) {
            int size;
            ctx >> size;
            for(auto i = 0; i < size; i++) {
				Key key = Key();
				Value value = Value();
				ctx >> checked(key) >> checked(value);
                result[key] = value;
            }
            return ctx;
        }

        template<typename Item>
        GwtParseContext &operator>>(GwtParseContext &ctx, std::vector<Item>& result) {
            int size;
            ctx >> size;
            for(auto i = 0; i < size; i++) {
				Item item;
				ctx >> checked(item);
                result.push_back(item);
            }
            return ctx;
        }

		template<typename Item>
		GwtParseContext &operator>>(GwtParseContext &ctx, std::set<Item>& result) {
			int size;
			ctx >> size;
			for (auto i = 0; i < size; i++) {
				Item item;
				ctx >> checked(item);
				result.insert(item);
			}
			return ctx;
		}
	}
}

namespace std {
    template<typename Key, typename Value>
    std::ostream& operator<<(std::ostream &stream, const std::map<Key, Value> &item) {
        for(auto v: item) {
            stream << "<pair><key>" << v.first << "</key><value>" << v.second << "</value></pair>" << std::endl;
        }
        return stream;
    }

    template<typename Item>
    std::ostream& operator<<(std::ostream &stream, const std::vector<Item> &item) {
        for(auto v: item) {
            stream << "<item>" << v << "</item>" << std::endl;
        }
        return stream;
    }

	template<typename Item>
	std::ostream& operator<<(std::ostream &stream, const std::set<Item> &item) {
		for (auto v : item) {
			stream << "<item>" << v << "</item>" << std::endl;
		}
		return stream;
	}

	template<typename Item>
	std::ostream& operator<<(std::ostream &stream, const std::shared_ptr<Item> &item) {
		stream << *item << std::endl;
		return stream;
	}
}


#endif //QT_SECOND_GWTPARSECONTEXT_H
