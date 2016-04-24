//
// Created by alexx on 26.03.2016.
//

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include "GwtType.h"
#include "GwtParser.h"
#include <QDomDocument>
#include "GwtIterator.h"

namespace vantagefx {
    namespace api {

		std::shared_ptr<GwtIterator> GwtType::iterateValues(GwtObjectPtr& object, GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path)
	    {
			return std::make_shared<GwtMapIterator>(object, it, end, path, _pointer);
	    }

	    GwtComplexType::GwtComplexType(const std::string &name,
                                       const std::string &primary,
                                       std::initializer_list<std::shared_ptr<GwtField>> fields)
                : GwtType(name),
                  _fields(fields),
                  _primary(primary)
        {
            auto last = 1;
            for (auto &field: _fields) {
                if (field->name().empty()) {
                    field->setName("some_" + boost::lexical_cast<std::string>(last++));
                }
            }
        }

        std::vector<std::shared_ptr<GwtField>> &GwtComplexType::fields()
        {
            return _fields;
        }

        void GwtComplexType::parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const
        {
            for (auto field: _fields) {
                auto fieldName = field->name();
				GwtValue value;
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
                result->addValue(fieldName, std::move(value));
            }
        }

	    std::shared_ptr<GwtField> GwtComplexType::field(std::string name)
	    {
		    for(auto fld: _fields) {
				if (fld->name() == name) return fld;
		    }
			return std::shared_ptr<GwtField>();
	    }

	    std::string GwtComplexType::primary() const
	    {
			return _primary;
	    }

	    std::shared_ptr<GwtIterator> GwtComplexType::iterateValues(GwtObjectPtr &object, GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path)
		{
			return std::make_shared<GwtComplexIterator>(object, _fields, it, end, path);
		}

	    void GwtComplexType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style)
        {
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

        void GwtComplexType::xml(GwtObject &object, QDomElement &parent)
        {
	        auto doc = parent.ownerDocument();
			parent.setAttribute("type", name().c_str());
            for (auto field: _fields) {
	            auto fieldName = field->name();
	            auto value = object.value(fieldName);
				if (!value.empty()) {
					auto prop = doc.createElement(fieldName.c_str());
					field->xml(value, prop);
					parent.appendChild(prop);
				}
            }
        }

        void GwtListType::parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const {
            int length;
            parser >> length;
            result->add("length", length);

            for (auto i = 0; i < length; i++) {
                auto name = boost::lexical_cast<std::string>(i);
                result->add(name, parser.parse());
            }
        }

	    std::shared_ptr<GwtIterator> GwtListType::iterateValues(GwtObjectPtr& object, GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path)
        {
			return std::make_shared<GwtArrayIterator>(object, it, end, path, _pointer);
        }

	    void GwtListType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style)
        {
            auto length = object.value("length");
            stream << "list[" << length.intValue() << "]";
        }

        void GwtListType::xml(GwtObject &object, QDomElement &parent)
        {
            auto length = object.value("length");
	        auto doc = parent.ownerDocument();
			parent.setAttribute("type", name().c_str());
			parent.setAttribute("count", length.intValue());

            for (auto i = 0; i < length.intValue(); i++) {
                auto item = doc.createElement("item");
                auto name = boost::lexical_cast<std::string>(i);
	            auto value = object.value(name);
				auto obj = value.objectValue();
                if (obj) obj->xml(item);
				parent.appendChild(item);
            }
        }

        void GwtMapType::parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const
        {
            for (auto i = 0; i < _skip; i++) {
                int tmp;
                parser >> tmp;
            }
            int length;
            parser >> length;

            for (auto i = 0; i < length; i++) {
                auto key = parser.parse();
                auto value = parser.parse();
				if (!key) {
					throw ParseError("null key not supported");
				}
                std::stringstream stream;
                key->print(stream, GwtPrintStyle::MapKey);
                result->add(stream.str(), value);
            }
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

		void GwtRequestType::parse(GwtParser& parser, std::shared_ptr<GwtObject>& result) const
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
				types.push_back(parser.word(typeId));
			}
	        auto i = 0;
			for(auto &type: types) {
				auto name = boost::lexical_cast<std::string>(i++);
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

	    void GwtRequestType::print(GwtObject& object, std::ostream& stream, GwtPrintStyle style)
        {
			stream << object.value("unit").stringValue();
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

            auto count = object.value("length").intValue();
            for(auto i = 0; i < count; i++) {
	            auto name = boost::lexical_cast<std::string>(i);
                auto param = doc.createElement("param");
                auto type = object.value("type_" + name).intValue();
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
                auto obj = pair.second.objectValue();
                if (obj) obj->xml(item);
				parent.appendChild(item);
            }
        }
    }
}
