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

        class GwtValue;

        class GwtObject;

        typedef std::shared_ptr<GwtValue> GwtValuePtr;

        class GwtValue {
        public:

            GwtValue();

            GwtValue(int value);

            GwtValue(int64_t value);

            GwtValue(double value);

            GwtValue(std::string value);

            GwtValue(std::shared_ptr<GwtObject> value);

			GwtValue(int value, std::string string);

            int intValue() const;

            double doubleValue() const;

            int64_t longValue() const;

            std::shared_ptr<GwtObject> &objectValue();

            const std::string &stringValue() const;

			int toInt() const;

			double toDouble() const;

			int64_t toLong() const;

			std::string toString() const;

			std::shared_ptr<GwtObject> toObject();

		private:
            boost::variant<int, double, int64_t, std::shared_ptr<GwtObject>> _value;
            std::string _string;
        };
    }
}


#endif //VANTAGEFX_GWTVALUE_H
