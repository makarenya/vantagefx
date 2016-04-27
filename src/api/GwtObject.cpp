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

        void GwtObject::print(std::ostream &stream, GwtPrintStyle style) const
		{
            _type->print(*this, stream, style);
        }

        void GwtObject::xml(QDomElement &element) const
		{
            _type->xml(*this, element);
        }

        void GwtObject::saveXml(fs::path filename) const
		{
            QDomDocument document;
            auto body = document.createElement("response");
            xml(body);
            document.appendChild(body);
            QFile fs(filename.string().c_str());
            fs.open(QIODevice::ReadWrite | QIODevice::Truncate);
            fs.write(document.toByteArray());
            fs.close();
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

		const std::map<std::string, GwtValue> &GwtObject::values() const {
			return _values;
		}

	    bool GwtObject::has(const std::string &name) const
        {
			return _values.find(name) != _values.end();
        }

	    GwtValue &GwtObject::value(const std::string &name) {
            return _values.at(name);
        }

		const GwtValue &GwtObject::value(const std::string &name) const {
			return _values.at(name);
		}

        std::shared_ptr<GwtIterator> GwtObject::iterateValues() const
        {
            return _type->iterateValues(shared_from_this());
        }

        GwtValue GwtObject::item(const std::string &path, std::initializer_list<GwtValue> &&values) const
        {
            return item(GwtPathExpression::parse(path), std::move(values));
        }

        GwtValue GwtObject::item(const GwtPath &path, std::initializer_list<GwtValue> &&values) const
        {
            GwtQueryIterator it(path, shared_from_this(), std::move(values), "");
            return it->value;
        }

	    GwtQuery GwtObject::query(const std::string &path, std::initializer_list<GwtValue> &&values) const
        {
			return query(GwtPathExpression::parse(path), std::move(values));
        }

	    GwtQuery GwtObject::query(const GwtPath &path, std::initializer_list<GwtValue> &&values) const
        {
			return GwtQuery(shared_from_this(), path, std::move(values));
        }
    }
}

