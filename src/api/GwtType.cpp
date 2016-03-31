//
// Created by alexx on 26.03.2016.
//

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "GwtType.h"
#include "GwtParser.h"
#include <QDomDocument>

namespace vantagefx {
    namespace api {

	    GwtComplexType::GwtComplexType(const std::string &name,
                                       const std::string &primary,
                                       std::initializer_list<std::shared_ptr<GwtField>> fields)
                : GwtType(name),
                  _fields(fields),
                  _primary(primary) {
            auto last = 1;
            for (auto &field: _fields) {
                if (field->name().empty()) {
                    field->setName("some_" + boost::lexical_cast<std::string>(last++));
                }
            }
        }

        std::vector<std::shared_ptr<GwtField>> &GwtComplexType::fields() {
            return _fields;
        }

        void GwtComplexType::parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) {
            for (auto field: _fields) {
                auto fieldName = field->name();
                GwtValuePtr value;
                try {
                    value = field->parse(parser);
                }
                catch (boost::bad_get) {
                    throw WrongGwtType(name(), fieldName, field->type(), parser.peekType());
                }
                if (result->has(fieldName)) {
                    throw ParseError("two objects with key '" + fieldName + "' while parsing " +
                                     parser.currentObject()->type()->name());
                }
                if (fieldName.empty()) {
                    throw ParseError("empty field name while parsing " + parser.currentObject()->type()->name());
                }
                result->addValue(fieldName, value);
            }
            _created.push_back(result);
        }

	    std::shared_ptr<GwtField> GwtComplexType::field(std::string name)
	    {
		    for(auto fld: _fields) {
				if (fld->name() == name) return fld;
		    }
			return std::shared_ptr<GwtField>();
	    }

	    void GwtComplexType::find(const GwtObject& object, const GwtValue& search, std::vector<std::string>& found, std::string prefix) const
        {
			for (auto field : _fields) {
				auto value = object.value(field->name());
				field->find(value, search, found, prefix + field->name());
			}
        }

	    GwtValuePtr GwtComplexType::get(GwtObject& object, const std::string& path)
	    {
			auto pos = std::find(path.begin(), path.end(), '/');
			auto fld = field(std::string(path.begin(), pos));
			if (!fld) throw std::runtime_error("bad path");
			auto value = object.value(fld->name());
			auto childPath = std::string();
			if (pos != path.end()) childPath = std::string(++pos, path.end());
			return fld->get(value, childPath);
	    }

	    void GwtComplexType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) {
            if (_primary.empty()) {
                stream << "complex";
            }
            else {
                for (auto field: _fields) {
                    if (field->name() != _primary) continue;
                    auto value = object.value(_primary);
                    field->print(value, stream, style);
                }
            }
        }

        void GwtComplexType::xml(GwtObject &object, QDomElement &parent) {
	        auto doc = parent.ownerDocument();
			parent.setAttribute("type", name().c_str());
            for (auto field: _fields) {
	            auto fieldName = field->name();
                auto prop = doc.createElement(fieldName.c_str());
	            auto value = object.value(fieldName);
                field->xml(value, prop);
                parent.appendChild(prop);
            }
        }

        void GwtComplexType::printTable(std::ostream &stream) {
	        auto first = true;
            for (auto field: _fields) {
                if (first) first = false;
                else stream << ";";
                stream << '"' << field->name() << field->factor() << '"';
            }
            stream << std::endl;
            for (auto objPtr: _created) {
                auto obj = objPtr.lock();
                if (!obj) continue;
                first = true;
                for (auto field: _fields) {
                    auto value = obj->value(field->name());
                    if (first) first = false;
                    else stream << ";";
                    stream << '"';
                    field->print(value, stream, GwtPrintStyle::CsvValue);
                    stream << '"';
                }
                stream << std::endl;
            }
        }

        void GwtListType::parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) {
            int length;
            parser >> length;
            result->add("length", length);

            for (auto i = 0; i < length; i++) {
                auto name = boost::lexical_cast<std::string>(i);
                result->add(name, parser.parse());
            }
        }

	    void GwtListType::find(const GwtObject& object, const GwtValue& search, std::vector<std::string>& found, std::string prefix) const
	    {
			auto length = object.value("length");

			for (auto i = 0; i < length->intValue(); i++) {
				auto name = boost::lexical_cast<std::string>(i);
				auto value = object.value(name);
				auto obj = value->objectValue();
				if (obj) obj->find(search, found, prefix + "[" + name + "]/");
			}
		}

	    std::shared_ptr<GwtValue> GwtListType::get(GwtObject& object, const std::string& path)
	    {
			auto pos = std::find(path.begin(), path.end(), '/');
			auto index = std::string(path.begin(), pos);
			if (index.length() < 3 || index[0] != '[' || index[index.length() - 1] != ']')
				throw std::runtime_error("bad path");
			auto fld = std::string(path.begin() + 1, pos - 1);
			auto value = object.value(fld);
			auto childPath = std::string();
			if (childPath.empty()) return value;
			auto obj = value->objectValue();
			if (obj) return obj->get(childPath);
			throw std::runtime_error("null object");
		}

	    void GwtListType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) {
            auto length = object.value("length");
            stream << "list[" << length->intValue() << "]";
        }

        void GwtListType::xml(GwtObject &object, QDomElement &parent) {
            auto length = object.value("length");
	        auto doc = parent.ownerDocument();
			parent.setAttribute("type", name().c_str());
			parent.setAttribute("count", length->intValue());

            for (auto i = 0; i < length->intValue(); i++) {
                auto item = doc.createElement("item");
                auto name = boost::lexical_cast<std::string>(i);
	            auto value = object.value(name);
				auto obj = value->objectValue();
                if (obj) obj->xml(item);
				parent.appendChild(item);
            }
        }

        void GwtMapType::parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) {
            for (auto i = 0; i < _skip; i++) {
                int tmp;
                parser >> tmp;
            }
            int length;
            parser >> length;

            for (auto i = 0; i < length; i++) {
                auto key = parser.parse();
                auto value = parser.parse();
                if (!key) throw ParseError("null key not supported");
                std::stringstream stream;
                key->print(stream, GwtPrintStyle::MapKey);
                result->add(stream.str(), value);
            }
        }

		void GwtMapType::find(const GwtObject& object, const GwtValue& search, std::vector<std::string>& found, std::string prefix) const
		{
			for (auto pair : object.values()) {
				auto obj = pair.second->objectValue();
				if (obj) obj->find(search, found, prefix + "[" + pair.first + "]/");
			}
		}

		std::shared_ptr<GwtValue> GwtMapType::get(GwtObject& object, const std::string& path)
		{
			auto pos = std::find(path.begin(), path.end(), '/');
			auto index = std::string(path.begin(), pos);
			if (index.length() < 3 || index[0] != '[' || index[index.length() - 1] != ']')
				throw std::runtime_error("bad path");
			auto fld = std::string(path.begin() + 1, pos - 1);
			auto value = object.value(fld);
			auto childPath = std::string();
			if (childPath.empty()) return value;
			auto obj = value->objectValue();
			if (obj) return obj->get(childPath);
			throw std::runtime_error("null object");
		}
		
		GwtRequestType::GwtRequestType()
			: GwtType("request")
	    {
            _fields = {
                fint("int"),
                fdbl("float"),
                flng("long"),
                fstr("string"),
                fptr("pointer")
            };
        }

		void GwtRequestType::parse(GwtParser& parser, std::shared_ptr<GwtObject>& result)
        {
			std::string unit;
			std::string hash;
			std::string iface;
			std::string method;
			int props;
			parser >> unit;
			parser >> hash;
			parser >> iface;
			parser >> method;
			parser >> props;
			result->add("unit", unit);
			result->add("hash", hash);
			result->add("interface", iface);
			result->add("method", method);
            result->add("length", props);
			std::vector<std::string> types;
			for (auto i = 0; i < props; i++) {
				int typeId;
				parser >> typeId;
				types.push_back(parser.typeName(typeId));
			}
            int i = 0;
			for(std::string &type: types) {
                std::string name = boost::lexical_cast<std::string>(i++);
				if (type == "Z" || type == "B" || type == "C" || type == "S" || type == "I") {
                    int value;
                    parser >> value;
                    result->add("value_" + name, value);
                    result->add("type_" + name, 0);
                }
                else if (type == "F" || type == "D") {
                    double value;
                    parser >> value;
                    result->add("value_" + name, value);
                    result->add("type_" + name, 1);
                }
                else if (type == "J") {
                    int64_t value;
                    parser >> value;
                    result->add("value_" + name, value);
                    result->add("type_" + name, 2);
                }
                else if (type == "java.lang.String") {
                    std::string value;
                    parser >> value;
                    result->add("value_" + name, value);
                    result->add("type_" + name, 3);
                }
                else {
                    result->add("value_" + name, parser.parse());
                    result->add("type_" + name, 4);
                }
			}
        }

		void GwtRequestType::find(const GwtObject& object, const GwtValue& search, std::vector<std::string>& found, std::string prefix) const
		{
		}

		std::shared_ptr<GwtValue> GwtRequestType::get(GwtObject& object, const std::string& path)
		{
			throw std::runtime_error("incomplete object");
		}

		void GwtRequestType::print(GwtObject& object, std::ostream& stream, GwtPrintStyle style)
        {
			stream << object.value("unit")->stringValue();
        }

	    void GwtRequestType::xml(GwtObject& object, QDomElement& parent)
	    {
            auto doc = parent.ownerDocument();
            auto unit = doc.createElement("unit");
            _fields[3]->xml(object.value("unit"), unit);
            parent.appendChild(unit);

            auto hash = doc.createElement("hash");
            _fields[3]->xml(object.value("hash"), hash);
            parent.appendChild(hash);

            auto iface = doc.createElement("interface");
            _fields[3]->xml(object.value("interface"), iface);
            parent.appendChild(iface);

            auto method = doc.createElement("method");
            _fields[3]->xml(object.value("method"), method);
            parent.appendChild(method);

            auto count = object.value("length")->intValue();
            for(auto i = 0; i < count; i++) {
                std::string name = boost::lexical_cast<std::string>(i);
                auto param = doc.createElement("param");
                auto type = object.value("type_" + name)->intValue();
                _fields[type]->xml(object.value("value_" + name), param);
                parent.appendChild(param);
            }
        }

	    void GwtMapType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) {
            stream << "map[" << object.values().size() << "]";
        }

        void GwtMapType::xml(GwtObject &object, QDomElement &parent) {
	        auto doc = parent.ownerDocument();
			parent.setAttribute("type", name().c_str());
			parent.setAttribute("count", static_cast<int>(object.values().size()));
            for (auto pair : object.values()) {
                auto item = doc.createElement("item");
                item.setAttribute("key", pair.first.c_str());
                auto obj = pair.second->objectValue();
                if (obj) obj->xml(item);
				parent.appendChild(item);
            }
        }

        GwtSimpleType::GwtSimpleType(std::string name, std::shared_ptr<GwtField> field)
                : GwtType(name),
                  _field(field) { }

        void GwtSimpleType::parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) {
            result->addValue("value", _field->parse(parser));
        }

		void GwtSimpleType::find(const GwtObject& object, const GwtValue& search, std::vector<std::string>& found, std::string prefix) const
		{
			auto value = object.value("value");
			_field->find(value, search, found, prefix);
		}

		std::shared_ptr<GwtValue> GwtSimpleType::get(GwtObject& object, const std::string& path)
		{
			auto value = object.value("value");
			return _field->get(value, path);
		}

		void GwtSimpleType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) {
	        auto value = object.value("value");
            _field->print(value, stream, style);
        }

        void GwtSimpleType::xml(GwtObject &object, QDomElement &parent) {
            std::stringstream stream;
	        auto value = object.value("value");
			_field->xml(value, parent);
        }
    }
}
