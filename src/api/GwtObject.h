//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTOBJECT_H
#define VANTAGEFX_GWTOBJECT_H


#include <memory>
#include <map>
#include "GwtField.h"
#include <boost/filesystem.hpp>

namespace vantagefx {
    namespace api {

        class GwtType;

        class GwtParser;

        class GwtValue;

        class GwtObject {
        public:
            explicit GwtObject(std::shared_ptr<GwtType> type);

            void print(std::ostream &stream, GwtPrintStyle style);

            void xml(QDomElement &element);

            std::map<std::string, std::shared_ptr<GwtValue>> values() const;

            std::shared_ptr<GwtValue> &value(const std::string &name);

            void addValue(std::string name, std::shared_ptr<GwtValue> value);

            std::shared_ptr<GwtType> type();

            bool has(std::string name);

            template<typename T>
            void add(std::string name, T value);

            void save(boost::filesystem::path file);

        private:
            std::shared_ptr<GwtType> _type;
            std::map<std::string, std::shared_ptr<GwtValue>> _values;
        };

        template<typename T>
        void GwtObject::add(std::string name, T value) {
            addValue(name, std::make_shared<GwtValue>(value, ""));
        }

        inline std::shared_ptr<GwtType> GwtObject::type() { return _type; }

        typedef std::shared_ptr<GwtObject> GwtObjectPtr;
    }
}


#endif //VANTAGEFX_GWTOBJECT_H
