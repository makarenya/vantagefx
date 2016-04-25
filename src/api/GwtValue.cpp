//
// Created by alexx on 26.03.2016.
//

#include "GwtValue.h"
#include "GwtParser.h"
#include "GwtObject.h"

namespace vantagefx {
    namespace api {

		struct int_visitor : boost::static_visitor<int>
		{
			int operator()(int value) const
			{
				return value;
			}
			int operator()(double value) const
			{
				if (std::abs(std::round(value) - value) < 0.001) 
					return static_cast<int>(std::round(value));
				return 0;
			}
			int operator()(int64_t value) const
			{
				if (value < INT_MAX && value > INT_MIN)
					return static_cast<int>(value);
				return 0;
			}
			int operator()(std::shared_ptr<GwtObject> value) const
			{
				return 0;
			}
		};

		struct long_visitor : boost::static_visitor<int64_t>
		{
			int64_t operator()(int value) const
			{
				return value;
			}
			int64_t operator()(double value) const
			{
				if (std::abs(std::round(value) - value) < 0.001)
					return static_cast<int64_t>(std::round(value));
				return 0;
			}
			int64_t operator()(int64_t value) const
			{
				return value;
			}
			int64_t operator()(std::shared_ptr<GwtObject> value) const
			{
				return 0;
			}
		};

		struct double_visitor : boost::static_visitor<double>
		{
			double operator()(int value) const
			{
				return value;
			}
			double operator()(double value) const
			{
				return value;
			}
			double operator()(int64_t value) const
			{
				return static_cast<double>(value);
			}
			double operator()(std::shared_ptr<GwtObject> value) const
			{
				return 0;
			}
		};

		struct string_visitor : boost::static_visitor<std::string>
		{
			std::string operator()(int value) const
			{
				return boost::lexical_cast<std::string>(value);
			}
			std::string operator()(double value) const
			{
				return boost::lexical_cast<std::string>(value);
			}
			std::string operator()(int64_t value) const
			{
				return boost::lexical_cast<std::string>(value);
			}
			std::string operator()(std::shared_ptr<GwtObject> value) const
			{
				return "";
			}
		};

		struct object_visitor : boost::static_visitor<GwtObjectPtr>
		{
			GwtObjectPtr operator()(int value) const
			{
				return GwtObjectPtr();
			}
			GwtObjectPtr operator()(double value) const
			{
				return GwtObjectPtr();
			}
			GwtObjectPtr operator()(int64_t value) const
			{
				return GwtObjectPtr();
			}
			GwtObjectPtr operator()(GwtObjectPtr value) const
			{
				return value;
			}
		};

		int GwtValue::intValue() const
		{
			return boost::get<int>(_value);
        }

        double GwtValue::doubleValue() const 
		{
			return boost::get<double>(_value);
		}

        int64_t GwtValue::longValue() const 
		{
			return boost::get<int64_t>(_value);
		}

        const std::shared_ptr<GwtObject> &GwtValue::objectValue() const
		{
			if (_value.empty()) return std::shared_ptr<GwtObject>();
            return boost::get<std::shared_ptr<GwtObject>>(_value);
        }

        const std::string &GwtValue::stringValue() const {
            return _string;
        }

	    int GwtValue::toInt() const
		{
			int_visitor visitor;
			auto result = boost::apply_visitor(visitor, _value);
			if (result == 0 && !_string.empty()) {
				try { result = boost::lexical_cast<int>(_string); }
				catch (boost::bad_lexical_cast) {}
			}
			return result;
		}

	    double GwtValue::toDouble() const
		{
			double_visitor visitor;
			auto result = boost::apply_visitor(visitor, _value);
			if (result == 0 && !_string.empty()) {
				try { result = boost::lexical_cast<double>(_string); }
				catch (boost::bad_lexical_cast) {}
			}
			return result;
		}

	    int64_t GwtValue::toLong() const
		{
			long_visitor visitor;
			auto result = boost::apply_visitor(visitor, _value);
			if (result == 0 && !_string.empty()) {
				try { result = boost::lexical_cast<int64_t>(_string); }
				catch (boost::bad_lexical_cast) {}
			}
			return result;
		}

	    bool GwtValue::empty() const
		{
			return _type == 0;
		}

	    std::string GwtValue::toString() const
		{
			if (!_string.empty()) return _string;
			string_visitor visitor;
			return boost::apply_visitor(visitor, _value);
		}

		GwtObjectPtr GwtValue::toObject()
		{
			object_visitor visitor;
			return boost::apply_visitor(visitor, _value);
		}

	    bool GwtValue::operator==(const GwtValue &rhs) const
	    {
			// 0 - null, 1 - число, 2 - строка, 3 - строка или число
			if (_type == 0 && rhs._type == 0) return true;

			if ((_type & 1) && (rhs._type & 1)) {
				if (_value == rhs._value) return true;
			}
			
			if ((_type & 2) && (rhs._type & 2)) {
				if (_string == rhs._string) return true;
			}

			return false;
		}

	    GwtValue::GwtValue(int value, std::string string)
			: _value(value),
			  _string(string),
			  _type(3) { }

        GwtValue::GwtValue(int value)
            : _value(value),
			  _type(1) { }

        GwtValue::GwtValue(int64_t value)
            : _value(value),
			  _type(1) { }

        GwtValue::GwtValue(double value)
            : _value(value),
			  _type(1) { }

        GwtValue::GwtValue(std::string value)
            : _string(value),
			  _type(2) { }

        GwtValue::GwtValue(std::shared_ptr<GwtObject> value)
            : _value(value),
			  _type(1) { }

        GwtValue::GwtValue()
            : _value(),
			  _type(0) { }
    }
}