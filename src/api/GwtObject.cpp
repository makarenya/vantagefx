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

	    std::string GwtObject::primary() const
        {
			if (_type->primary().empty()) return std::string();
			return value(_type->primary())->stringValue();
        }

	    void GwtObject::find(const GwtValue &value, std::vector<std::string> &found, std::string prefix) const
	    {
			return _type->find(*this, value, found, prefix);
        }

	    std::shared_ptr<GwtValue> GwtObject::get(const std::string& path)
        {
			return _type->get(*this, path);
        }

	    bool GwtObject::operator==(const GwtObject& other) const
	    {
			return _values == other._values;
		}

	    void GwtObject::addValue(std::string name, std::shared_ptr<GwtValue> value) {
            _values[name] = value;
        }

        std::map<std::string, std::shared_ptr<GwtValue>> GwtObject::values() const {
            return _values;
        }

        std::shared_ptr<GwtValue> &GwtObject::value(const std::string &name) {
            return _values[name];
        }

		const std::shared_ptr<GwtValue> &GwtObject::value(const std::string &name) const {
			return _values.at(name);
		}
	}
}

