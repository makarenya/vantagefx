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
#include <iterator>

namespace vantagefx {
    namespace api {

        class GwtType;

        class GwtParser;

        class GwtIterator;

        class GwtObject {
        public:
            explicit GwtObject(std::shared_ptr<GwtType> type);

            void print(std::ostream &stream, GwtPrintStyle style);

            void xml(QDomElement &element);

            void saveXml(boost::filesystem::path filename);

            std::map<std::string, GwtValue> &values();

            GwtValue &value(const std::string &name);

			const GwtValue &value(const std::string &name) const;

            void addValue(std::string name, GwtValue &&value);

            std::shared_ptr<GwtType> type() const;

            bool has(std::string name);

			std::string primary() const;

			bool operator==(const GwtObject &other) const;

            template<typename T>
            void add(std::string name, T value);

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
    }
}


#endif //VANTAGEFX_GWTOBJECT_H
