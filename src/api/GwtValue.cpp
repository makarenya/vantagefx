//
// Created by alexx on 26.03.2016.
//

#include "GwtValue.h"
#include "GwtParser.h"
#include "GwtObject.h"

namespace vantagefx {
    namespace api {

        class LongValue : public GwtValue {
        public:
            void parse(GwtParser &ctx) override;
            void print(std::ostream &stream) override;
            GwtValueType type() override;
            int value() override;
        private:
            int64_t _value;
        };

        class DateValue : public LongValue {
        public:
            void print(std::ostream &stream) override;
            GwtValueType type() override;
        private:
            int64_t _value;
        };

        class StdValue : public GwtValue {
            void parse(GwtParser &ctx) override;
            void print(std::ostream &stream) override;
            GwtValueType type() override;
            int value() override;
        private:
            int _value;
            std::string _string;
        };

        class IntValue : public GwtValue {
            void parse(GwtParser &ctx) override;
            void print(std::ostream &stream) override;
            GwtValueType type() override;
            int value() override;
        private:
            int _value;
        };

        class FloatValue : public GwtValue {
            void parse(GwtParser &ctx) override;
            void print(std::ostream &stream) override;
            GwtValueType type() override;
            int value() override;
        private:
            double _value;
        };

        class StringValue : public GwtValue {
            void parse(GwtParser &ctx) override;
            void print(std::ostream &stream) override;
            GwtValueType type() override;
            int value() override;
        private:
            int _value;
            std::string _string;
        };

        class PointerValue : public GwtValue {
            void parse(GwtParser &ctx) override;
            void print(std::ostream &stream) override;
            GwtValueType type() override;
            int value() override;
        private:
            std::shared_ptr<GwtObject> _object;
        };
    }

    namespace api {

        std::string formatXml(std::string value) {
            boost::algorithm::replace_all(value, ">", "&gt;");
            boost::algorithm::replace_all(value, "<", "&lt;");
            return value;
        }

        GwtValuePtr GwtValue::parse(GwtParser &ctx, GwtValueType type) {
            using std::static_pointer_cast;
            using std::make_shared;
            GwtValuePtr ptr;
            switch(type) {
                case GwtValueType::Long:
                    ptr = static_pointer_cast<GwtValue>(make_shared<LongValue>());
                    break;
                case GwtValueType::Std:
                    ptr = static_pointer_cast<GwtValue>(make_shared<StdValue>());
                    break;
                case GwtValueType::Integer:
                    ptr = static_pointer_cast<GwtValue>(make_shared<IntValue>());
                    break;
                case GwtValueType::Float:
                    ptr = static_pointer_cast<GwtValue>(make_shared<FloatValue>());
                    break;
                case GwtValueType::String:
                    ptr = static_pointer_cast<GwtValue>(make_shared<StringValue>());
                    break;
                case GwtValueType::Pointer:
                    ptr = static_pointer_cast<GwtValue>(make_shared<PointerValue>());
                    break;
                case GwtValueType::Date:
                    ptr = static_pointer_cast<GwtValue>(make_shared<DateValue>());
                    break;
            }
            ptr->parse(ctx);
            return ptr;
        }


        void LongValue::parse(GwtParser &ctx) {
            ctx >> _value;
        }

        void LongValue::print(std::ostream &stream) {
            using boost::posix_time::ptime;
            using boost::posix_time::milliseconds;
            namespace gregorian = boost::gregorian;
            ptime start(gregorian::date(1970, 1, 1));
            auto date = start + milliseconds(_value);
            ptime limit(gregorian::date(2000, 1, 1));
            if (date > limit)
                stream << _value << " [" << date << "]";
            else
                stream << _value;
        }

        GwtValueType LongValue::type() {
            return GwtValueType::Long;
        }

        int LongValue::value() {
            return static_cast<int>(_value);
        }

        void DateValue::print(std::ostream &stream) {
            using boost::posix_time::ptime;
            using boost::posix_time::milliseconds;
            namespace gregorian = boost::gregorian;
            ptime start(gregorian::date(1970, 1, 1));
            auto date = start + milliseconds(_value);
            stream << date;
        }

        GwtValueType DateValue::type() {
            return GwtValueType::Date;
        }

        void StdValue::parse(GwtParser &ctx) {
            ctx >> _value;
            _string  = ctx.str(_value);
        }

        void StdValue::print(std::ostream &stream) {
			if (_value <= 1) stream << _value;
            else stream << _value << " [" << formatXml(_string) << "]";
        }

        GwtValueType StdValue::type() {
            return GwtValueType::Std;
        }

        int StdValue::value() {
            return _value;
        }

        void IntValue::parse(GwtParser &ctx) {
            *static_cast<GwtParseContext*>(&ctx) >> _value;
        }

        void IntValue::print(std::ostream &stream) {
            stream << _value;
        }

        GwtValueType IntValue::type() {
            return GwtValueType::Integer;
        }

        int IntValue::value() {
            return _value;
        }


        void FloatValue::parse(GwtParser &ctx) {
            *static_cast<GwtParseContext*>(&ctx) >> _value;
        }

        void FloatValue::print(std::ostream &stream) {
            stream << _value;
        }

        GwtValueType FloatValue::type() {
            return GwtValueType::Float;
        }

        int FloatValue::value() {
            return (int)_value;
        }

        void StringValue::parse(GwtParser &ctx) {
            ctx >> _value;
            _string = ctx.word(_value);
        }

        void StringValue::print(std::ostream &stream) {
            stream << formatXml(_string);
        }

        GwtValueType StringValue::type() {
            return GwtValueType::String;
        }

        int StringValue::value() {
            return _value;
        }

        void PointerValue::parse(GwtParser &ctx) {
            _object = ctx.parse();
        }

        void PointerValue::print(std::ostream &stream) {
			if (_object) {
				_object->print(stream, GwtPrintStyle::Brief);
			}
        }

        GwtValueType PointerValue::type() {
            return GwtValueType::Pointer;
        }

        int PointerValue::value() {
            return 0;
        }
    }
}


