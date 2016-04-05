//
// Created by alexx on 26.03.2016.
//

#include "GwtField.h"
#include "GwtObject.h"
#include "GwtParser.h"
#include "GwtValue.h"
#include "GwtType.h"
#include <QDomElement>

namespace vantagefx {
    namespace api {

        class GwtIntField : public GwtField {
        public:
	        explicit GwtIntField(const std::string &name);

            std::string factor() const override;

            void notOrdered();

            void notUnique();

            void notSingle();

        private:
            bool _ordered;
            bool _unique;
            bool _single;
        };

        class LongField : public GwtIntField {
        public:
	        explicit LongField(const std::string &name);

            GwtValuePtr parse(GwtParser &parser) override;

            void print(GwtValuePtr &, std::ostream &stream, GwtPrintStyle style) const override;

            std::string type() const override { return "long"; }

            void setLast(int64_t value) { _last = value; }

            std::vector<int64_t> &values() { return _values; }

            int64_t last() const { return _last; }

			bool equal(const GwtValue &value, const GwtValue &other) const override;
		
		private:
            std::vector<int64_t> _values;
            int64_t _last = INT64_MIN;
        };

        class IntField : public GwtIntField {
        public:
	        explicit IntField(const std::string &name);

            GwtValuePtr parse(GwtParser &parser) override;

            void print(GwtValuePtr &, std::ostream &stream, GwtPrintStyle style) const override;

            std::string type() const override { return "int"; }

            void setLast(int value) { _last = value; }

            std::vector<int> &values() { return _values; }

            int last() const { return _last; }

			bool equal(const GwtValue &value, const GwtValue &other) const override;
		
		private:
            std::vector<int> _values;
            int _last = INT_MIN;
        };

        class StdField : public IntField {
        public:
	        explicit StdField(const std::string &name);

            GwtValuePtr parse(GwtParser &parser) override;

            void print(GwtValuePtr &, std::ostream &stream, GwtPrintStyle style) const override;

            std::string type() const override { return "std"; }

			bool equal(const GwtValue &value, const GwtValue &other) const override;
		};

        class DateField : public LongField {
        public:
	        explicit DateField(std::string name);

            void print(GwtValuePtr &, std::ostream &stream, GwtPrintStyle style) const override;

            std::string type() const override { return "date"; }
        };

        class FloatField : public GwtIntField {
        public:
	        explicit FloatField(const std::string &name);

            GwtValuePtr parse(GwtParser &parser) override;

            void print(GwtValuePtr &, std::ostream &stream, GwtPrintStyle style) const override;

            std::string type() const override { return "float"; }

            void setLast(double value) { _last = value; }

            std::vector<double> &values() { return _values; }

            double last() const { return _last; }

			bool equal(const GwtValue &value, const GwtValue &other) const override;
		
		private:
            std::vector<double> _values;
            double _last = INT_MIN;
        };

        class StringField : public GwtIntField {
        public:
	        explicit StringField(const std::string &name);

            GwtValuePtr parse(GwtParser &parser) override;

            void print(GwtValuePtr &, std::ostream &stream, GwtPrintStyle style) const override;

            std::string type() const override { return "string"; }

            void setLast(std::string value) { _last = value; }

            std::vector<std::string> &values() { return _values; }

            std::string last() const { return _last; }

			bool equal(const GwtValue &value, const GwtValue &other) const override;

		private:
            std::vector<std::string> _values;
            std::string _last;
        };

        class PtrField : public GwtIntField {
        public:
	        explicit PtrField(const std::string &name);

            GwtValuePtr parse(GwtParser &parser) override;

            void xml(GwtValuePtr &value, QDomElement &element) const override;

            void print(GwtValuePtr &, std::ostream &stream, GwtPrintStyle style) const override;

            std::string type() const override { return "ptr"; }

			bool equal(const GwtValue &value, const GwtValue &other) const override;
		};

        std::shared_ptr<GwtField> flng(const std::string &name) {
            return std::make_shared<LongField>(name);
        }

        std::shared_ptr<GwtField> fint(const std::string &name) {
            return std::make_shared<IntField>(name);
        }

        std::shared_ptr<GwtField> fstd(const std::string &name) {
            return std::make_shared<StdField>(name);
        }

        std::shared_ptr<GwtField> fdbl(const std::string &name) {
            return std::make_shared<FloatField>(name);
        }

        std::shared_ptr<GwtField> fdte(const std::string &name) {
            return std::make_shared<DateField>(name);
        }

        std::shared_ptr<GwtField> fstr(const std::string &name) {
            return std::make_shared<StringField>(name);
        }

        std::shared_ptr<GwtField> fptr(const std::string &name) {
            return std::make_shared<PtrField>(name);
        }
    }
    namespace api {

        template<typename Field, typename Value>
        void updateFactor(Field *field, Value &value) {
            if (value < field->last()) field->notOrdered();
            else field->setLast(value);
            if (std::find(field->values().begin(), field->values().end(), value) == field->values().end())
                field->values().push_back(value);
            else field->notUnique();
            if (field->values().size() > 1) field->notSingle();
        }

        GwtField::GwtField(const std::string &name)
                : _name(name) { }

        void GwtField::xml(GwtValuePtr &value, QDomElement &parent) const {
            std::stringstream stream;
            print(value, stream, GwtPrintStyle::XmlFieldValue);
            auto doc = parent.ownerDocument();
            auto text = doc.createTextNode(stream.str().c_str());
            parent.appendChild(text);
        }

	    void GwtField::setName(const std::string &name) {
            _name = name;
        }

        std::string GwtField::name() const {
            return _name;
        }

        GwtIntField::GwtIntField(const std::string &name)
                : GwtField(name),
                  _ordered(true),
                  _unique(true),
                  _single(true) { }

        inline void GwtIntField::notSingle() { _single = false; }

        inline void GwtIntField::notUnique() { _unique = false; }

        inline void GwtIntField::notOrdered() { _ordered = false; }

        std::string GwtIntField::factor() const {
            std::string result = "";
            if (_unique) result += "U";
            if (_ordered) result += "O";
            if (_single) result += "S";
            if (result.empty()) return "";
            return " (" + result + ")";
        }

        LongField::LongField(const std::string &name)
                : GwtIntField(name) { }

        GwtValuePtr LongField::parse(GwtParser &parser) {
            int64_t value;
            parser >> value;
            updateFactor(this, value);
            return std::make_shared<GwtValue>(value);
        }

        void LongField::print(GwtValuePtr &ptr, std::ostream &stream, GwtPrintStyle style) const {
            using boost::posix_time::ptime;
            using boost::posix_time::milliseconds;
            namespace gregorian = boost::gregorian;
            ptime start(gregorian::date(1970, 1, 1));
            auto date = start + milliseconds(ptr->longValue());
            ptime limit(gregorian::date(2000, 1, 1));
            if (date > limit)
                stream << ptr->longValue() << " [" << date << "]";
            else
                stream << ptr->longValue();
        }

	    bool IntField::equal(const GwtValue& value, const GwtValue& other) const
        {
			int o = other.toInt();
			return value.intValue() == other.toInt() && value.intValue() != 0;
		}

	    StdField::StdField(const std::string &name)
                : IntField(name) { }

        GwtValuePtr StdField::parse(GwtParser &parser) {
            int value;
            parser >> value;
            auto string = parser.str(value);
            if (value == parser.maxWord()) {
                std::cout << parser.currentObject()->type()->name() << "::" << name() << " can be a string '"
                << string << "'" << std::endl;
            }
            updateFactor(this, value);
            return std::make_shared<GwtValue>(value, string);
        }

        void StdField::print(GwtValuePtr &ptr, std::ostream &stream, GwtPrintStyle style) const {
            int value = ptr->intValue();
            if (value <= 1) stream << value;
            else stream << value << " [" << ptr->stringValue() << "]";
        }

	    bool LongField::equal(const GwtValue& value, const GwtValue& other) const
        {
			return value.longValue() == other.toLong() && value.longValue() != 0;
		}

	    IntField::IntField(const std::string &name)
                : GwtIntField(name) { }

        GwtValuePtr IntField::parse(GwtParser &parser) {
            int value;
            parser >> value;
            updateFactor(this, value);
            return std::make_shared<GwtValue>(value);
        }

        void IntField::print(GwtValuePtr &ptr, std::ostream &stream, GwtPrintStyle style) const {
            stream << ptr->intValue();
        }

	    bool StdField::equal(const GwtValue& value, const GwtValue& other) const
        {
			if (value.intValue() == other.toInt() && value.intValue() != 0) return true;
			return value.stringValue() == other.toString() && !value.stringValue().empty();
        }

	    DateField::DateField(std::string name)
                : LongField(name) { }

        void DateField::print(GwtValuePtr &ptr, std::ostream &stream, GwtPrintStyle style) const {
            using boost::posix_time::ptime;
            using boost::posix_time::milliseconds;
            namespace gregorian = boost::gregorian;
            ptime start(gregorian::date(1970, 1, 1));
            auto date = start + milliseconds(ptr->longValue());
            stream << date;
        }

        FloatField::FloatField(const std::string &name)
                : GwtIntField(name) { }

        GwtValuePtr FloatField::parse(GwtParser &parser) {
            double value;
            parser >> value;
            updateFactor(this, value);
            return std::make_shared<GwtValue>(value);
        }

        void FloatField::print(GwtValuePtr &ptr, std::ostream &stream, GwtPrintStyle style) const {
            stream << ptr->doubleValue();
        }

	    bool FloatField::equal(const GwtValue& value, const GwtValue& other) const
        {
			return value.doubleValue() == other.toDouble() && value.doubleValue() != 0.0;
        }

	    StringField::StringField(const std::string &name)
                : GwtIntField(name) { }

        GwtValuePtr StringField::parse(GwtParser &parser) {
            int value;
            parser >> value;
            auto string = parser.word(value);
            updateFactor(this, string);
            return std::make_shared<GwtValue>(value, string);
        }

        void StringField::print(GwtValuePtr &ptr, std::ostream &stream, GwtPrintStyle style) const {
            stream << ptr->stringValue();
        }

	    bool StringField::equal(const GwtValue& value, const GwtValue& other) const
        {
			return value.stringValue() == other.toString() && !value.stringValue().empty();
        }

	    PtrField::PtrField(const std::string &name)
                : GwtIntField(name) { }

        GwtValuePtr PtrField::parse(GwtParser &parser) {
            return std::make_shared<GwtValue>(parser.parse());
        }

        void PtrField::xml(GwtValuePtr &value, QDomElement &element) const {
            auto object = value->objectValue();
            if (!object) return;
            object->xml(element);
        }

        void PtrField::print(GwtValuePtr &ptr, std::ostream &stream, GwtPrintStyle style) const {
            auto object = ptr->objectValue();
            if (!object) return;
            object->print(stream, style);
        }

	    bool PtrField::equal(const GwtValue& value, const GwtValue& other) const
        {
			return false;
        }
    }
}
