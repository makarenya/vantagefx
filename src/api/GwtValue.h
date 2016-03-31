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
            template<typename T>
            explicit GwtValue(T value, std::string str = "");

            int intValue() const;

            double doubleValue() const;

            int64_t longValue() const;

            std::shared_ptr<GwtObject> &objectValue();

            const std::string &stringValue() const;

			bool operator==(const GwtValue &second) const;

        private:
            boost::variant<int, double, int64_t, std::shared_ptr<GwtObject>> _value;
            std::string _string;
        };

        template<typename T>
        GwtValue::GwtValue(T value, std::string str)
                : _value(value),
                  _string(str) { }
    }
}


#endif //VANTAGEFX_GWTVALUE_H
