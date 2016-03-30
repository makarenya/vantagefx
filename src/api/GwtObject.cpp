//
// Created by alexx on 26.03.2016.
//

#include "GwtObject.h"
#include "GwtType.h"
#include "ParseError.h"
#include <QDomDocument>
#include <QFile>

namespace vantagefx {
    namespace api {

        namespace fs = boost::filesystem;

        GwtObject::GwtObject(std::shared_ptr<GwtType> type)
                : _type(type) { }

        void GwtObject::print(std::ostream &stream, GwtPrintStyle style) {
            _type->print(*this, stream, style);
        }

        void GwtObject::xml(QDomElement &element) {
            _type->xml(*this, element);
        }

        bool GwtObject::has(std::string name) {
            auto it = _values.find(name);
            return it != _values.end();
        }

        void GwtObject::addValue(std::string name, std::shared_ptr<GwtValue> value) {
            _values[name] = value;
        }

        std::map<std::string, std::shared_ptr<GwtValue>> GwtObject::values() const {
            return _values;
        }

        std::shared_ptr<GwtValue> GwtObject::value(const std::string &name) {
            return _values[name];
        }

        void GwtObject::save(fs::path file) {
            QDomDocument document;
	        auto body = document.createElement("request");
            xml(body);
            document.appendChild(body);

            QFile fs(file.string().c_str());
            fs.open(QIODevice::ReadWrite | QIODevice::Truncate);
            fs.write(document.toByteArray());
            fs.close();
        }
    }
}

