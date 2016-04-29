//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTOBJECT_H
#define VANTAGEFX_GWTOBJECT_H


#include <memory>
#include <map>
#include "GwtField.h"
#include "GwtPathExpression.h"
#include <boost/filesystem.hpp>
#include "GwtValue.h"
#include "GwtIterator.h"
#include <iterator>

namespace vantagefx {
    namespace api {
	    class GwtHttpRequestContext;

	    class GwtType;

        class GwtParser;

        class GwtIterator;

		class GwtQuery;

        class GwtObject : public std::enable_shared_from_this<GwtObject> {
        public:
            explicit GwtObject(std::shared_ptr<GwtType> type);

            void serialize(GwtHttpRequestContext &ctx) const;

            void print(std::ostream &stream, GwtPrintStyle style) const;

            void xml(QDomElement &element) const;

            void saveXml(boost::filesystem::path filename) const;

            std::map<std::string, GwtValue> &values();

			const std::map<std::string, GwtValue> &values() const;

			bool has(const std::string &name) const;

            GwtValue &value(const std::string &name);

			const GwtValue &value(const std::string &name) const;

            void addValue(std::string name, GwtValue &&value);

            std::shared_ptr<GwtType> type() const;

			std::string primary() const;

			bool operator==(const GwtObject &other) const;

            template<typename T>
            void add(std::string name, T value);

            std::shared_ptr<GwtIterator> iterateValues() const;

            GwtValue item(const std::string &path, std::initializer_list<GwtValue> &&values = {}) const;

            GwtValue item(const GwtPath &path, std::initializer_list<GwtValue> &&values = {}) const;

			GwtQuery query(const std::string &path, std::initializer_list<GwtValue> &&values = {}) const;

			GwtQuery query(const GwtPath &path, std::initializer_list<GwtValue> &&values = {}) const;

        private:
            std::shared_ptr<GwtType> _type;
            std::map<std::string, GwtValue> _values;
        };

        template<typename T>
        void GwtObject::add(std::string name, T value) {
            addValue(name, GwtValue(value));
        }

        template<>
        inline void GwtObject::add(std::string name, std::string value) {
            addValue(name, GwtValue(0, value));
        }

        inline std::shared_ptr<GwtType> GwtObject::type() const
        { return _type; }

        typedef std::shared_ptr<GwtObject> GwtObjectPtr;

        typedef std::shared_ptr<const GwtObject> GwtConstObjectPtr;
    }
}


#endif //VANTAGEFX_GWTOBJECT_H
