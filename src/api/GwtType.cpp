//
// Created by alexx on 26.03.2016.
//

#include <boost/lexical_cast.hpp>
#include "GwtType.h"
#include "GwtObject.h"
#include "GwtParser.h"
#include "GwtField.h"
#include <iostream>

namespace vantagefx {
    namespace api {

        GwtComplexType::GwtComplexType(std::string name, std::vector<GwtField> fields, int primary)
                : GwtType(name),
                  _fields(fields),
                  _primary(primary) {
            auto last = 1;
            for (auto &field: _fields) {
                if (field.name().empty()) {
                    field.setName("some_" + boost::lexical_cast<std::string>(last++));
                }
            }
        }

        std::vector<GwtField> GwtComplexType::fields() const {
            return _fields;
        }

        std::shared_ptr<GwtObject> GwtComplexType::parse(GwtParser &parser) {
            auto obj = std::make_shared<GwtObject>(shared_from_this());

            for (auto &field: _fields) {
                auto fieldName = field.name();
                GwtValuePtr value;
                try {
                    value = GwtValue::parse(parser, field.type());
                }
                catch (boost::bad_get) {
                    std::cout << name() << "::" << fieldName << " parsed as "
                    << static_cast<int>(field.type())
                    << " but actual type is " << parser.peekType() << std::endl;
                    throw;
                }
                if (value->type() == GwtValueType::Std && value->value() == parser.maxWord()) {
                    std::cout << name() << "::" << fieldName << " can be a string" << std::endl;
                }
                obj->addValue(fieldName, value);
            }
            _created.push_back(obj);
            return obj;
        }

        void GwtComplexType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) {
            if (style == GwtPrintStyle::Text) {
                if (_primary >= 0)
                    object.value(_fields[_primary].name())->print(stream);
                else
                    stream << "complex";
            }
            else {
                stream << "<object type=\"" << name() << "\">";
                for (GwtField &field: _fields) {
                    stream << "<" << field.name() << ">";
                    object.value(field.name())->print(stream);
                    stream << "</" << field.name() << ">";
                }
                stream << "</object>";
            }
        }

        void GwtComplexType::printTable(std::ostream &stream) {
            std::map<std::string, std::string> factors;

            for (GwtField &field: _fields) {
                std::string factor = "";
                if (field.type() == GwtValueType::Integer
                    || field.type() == GwtValueType::Std
                    || field.type() == GwtValueType::String) {
                    std::vector<int> values;
                    bool unique = true;
                    bool ordered = true;
                    int last = -10000;
                    for (auto objPtr: _created) {
                        auto obj = objPtr.lock();
                        if (!obj) continue;

                        int intval = obj->value(field.name())->value();
                        if (intval <= last) ordered = false;
                        else last = intval;
                        if (std::find(values.begin(), values.end(), intval) == values.end())
                            values.push_back(intval);
                        else unique = false;
                    }
                    if (values.size() == 2
                        && (values[0] == 0 || values[0] == 1)
                        && (values[1] == 0 || values[1] == 1))
                        factor += "B";
                    if (values.size() <= 1) factor += "S";
                    if (unique) factor += "U";
                    if (ordered) factor += "O";
                }
                if (!factor.empty()) factor = " (" + factor + ")";
                factors[field.name()] = factor;
            }

            bool first = true;
            for (GwtField &field: _fields) {
                if (first) first = false;
                else stream << ";";
                stream << '"' << field.name() << factors[field.name()] << '"';
            }
            stream << std::endl;
            for (auto objPtr: _created) {
                auto obj = objPtr.lock();
                if (!obj) continue;
                first = true;
                for (GwtField &field: _fields) {
                    auto value = obj->value(field.name());
                    if (first) first = false;
                    else stream << ";";
                    stream << '"';
                    if (value->type() == GwtValueType::Pointer) {
                        stream << "<object>";
                    }
                    else {
                        value->print(stream);
                    }
                    stream << '"';
                }
                stream << std::endl;
            }
        }


        std::shared_ptr<GwtObject> GwtListType::parse(GwtParser &parser) {
            auto obj = std::make_shared<GwtObject>(shared_from_this());

            auto length = GwtValue::parse(parser, GwtValueType::Integer);
            obj->addValue("length", length);

            for (auto i = 0; i < length->value(); i++) {
                auto name = boost::lexical_cast<std::string>(i);
                obj->addValue(name, GwtValue::parse(parser, GwtValueType::Pointer));
            }
            return obj;
        }

        void GwtListType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) {
            auto length = object.value("length");
            if (style == GwtPrintStyle::Text) {
                stream << "list[" << length->value() << "]";
            }
            else {
                stream << "<object type=\"" << name() << "\" count=\"" << length->value() << "\">";

                for (auto i = 0; i < length->value(); i++) {
                    auto name = boost::lexical_cast<std::string>(i);
                    object.value(name)->print(stream);
                }
                stream << "</object>";
            }
        }

        std::shared_ptr<GwtObject> GwtMapType::parse(GwtParser &parser) {
            auto obj = std::make_shared<GwtObject>(shared_from_this());
            for (auto i = 0; i < _skip; i++) {
                GwtValue::parse(parser, GwtValueType::Integer);
            }
            auto length = GwtValue::parse(parser, GwtValueType::Integer);

            for (auto i = 0; i < length->value(); i++) {
                auto key = parser.parse();
                auto value = GwtValue::parse(parser, GwtValueType::Pointer);
                std::stringstream stream;
                key->print(stream, GwtPrintStyle::Text);
                obj->addValue(stream.str(), value);
            }

            return obj;
        }

        void GwtMapType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) {
            if (style == GwtPrintStyle::Text) {
                stream << "map[" << object.values().size() << "]";
            }
            else {
                stream << "<object type=\"" << name() << "\" count=\"" << object.values().size() << "\">";

                for (auto pair : object.values()) {
                    stream << "<item key=\"" << pair.first << "\">";
                    pair.second->print(stream);
                    stream << "</item>";
                }
                stream << "</object>";
            }
        }

        GwtSimpleType::GwtSimpleType(std::string name, GwtValueType type)
                : GwtType(name),
                  _type(type) { }

        std::shared_ptr<GwtObject> GwtSimpleType::parse(GwtParser &parser) {
            auto obj = std::make_shared<GwtObject>(shared_from_this());
            obj->addValue("value", GwtValue::parse(parser, _type));
            return obj;
        }

        void GwtSimpleType::print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) {
            if (style == GwtPrintStyle::Text || style == GwtPrintStyle::Brief) {
                object.value("value")->print(stream);
            }
            else {
                std::string name;
                switch (_type) {
                    case GwtValueType::Integer:
                        name = "integer";
                        break;
                    case GwtValueType::Float:
                        name = "float";
                        break;
                    case GwtValueType::Date:
                        name = "date";
                        break;
                    case GwtValueType::Long:
                        name = "long";
                        break;
                    case GwtValueType::String:
                        name = "string";
                        break;
                    default:
                        name = "value";
                        break;
                }
                stream << "<" << name << ">";
                object.value("value")->print(stream);
                stream << "</" << name << ">";
            }
        }
    }
}



