//
// Created by alexx on 26.03.2016.
//

#include "GwtObject.h"
#include "GwtType.h"
#include "ParseError.h"
#include "GwtIterator.h"
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

        void GwtObject::saveXml(fs::path filename) {
            QDomDocument document;
            auto body = document.createElement("response");
            xml(body);
            document.appendChild(body);
            QFile fs(filename.string().c_str());
            fs.open(QIODevice::ReadWrite | QIODevice::Truncate);
            fs.write(document.toByteArray());
            fs.close();
        }


        bool GwtObject::has(std::string name) {
            auto it = _values.find(name);
            return it != _values.end();
        }

	    std::string GwtObject::primary() const
        {
			if (_type->primary().empty()) return std::string();
			return value(_type->primary()).stringValue();
        }

	    bool GwtObject::operator==(const GwtObject& other) const
	    {
			return _values == other._values;
		}

	    void GwtObject::addValue(std::string name, GwtValue &&value) {
            _values[name] = value;
        }

        std::map<std::string, GwtValue> &GwtObject::values() {
            return _values;
        }

		GwtValue &GwtObject::value(const std::string &name) {
            return _values.at(name);
        }

		const GwtValue &GwtObject::value(const std::string &name) const {
			return _values.at(name);
		}
    }
}

