//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTVALUE_H
#define VANTAGEFX_GWTVALUE_H


#include <iostream>
#include <memory>
#include <boost/variant.hpp>

namespace vantagefx {
    namespace api {

        class GwtParser;

        class GwtObject;

        class GwtValue {
        public:

            GwtValue();

            explicit GwtValue(int value);

			explicit GwtValue(int64_t value);

			explicit GwtValue(double value);

			explicit GwtValue(std::string value);

			explicit GwtValue(std::shared_ptr<GwtObject> value);

			GwtValue(int value, std::string string);

            int intValue() const;

            double doubleValue() const;

            int64_t longValue() const;

            const std::shared_ptr<GwtObject> &objectValue() const;

            const std::string &stringValue() const;

			int toInt() const;

			double toDouble() const;

			int64_t toLong() const;

			bool empty() const;

			std::string toString() const;

			std::shared_ptr<GwtObject> toObject();

			bool operator==(const GwtValue &rhs) const;

			bool operator!=(const GwtValue &rhs) const;

		private:
            boost::variant<int, double, int64_t, std::shared_ptr<GwtObject>> _value;
            std::string _string;
			int _type;
        };

		inline bool GwtValue::operator!=(const GwtValue &rhs) const { return !(rhs == *this); }
    }
}


#endif //VANTAGEFX_GWTVALUE_H
