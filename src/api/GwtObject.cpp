//
// Created by alexx on 26.03.2016.
//

#include "GwtObject.h"
#include "GwtType.h"

namespace vantagefx {
    namespace api {

        GwtObject::GwtObject(std::shared_ptr<GwtType> type) {
            _type = type;
        }

        void GwtObject::print(std::ostream &stream, GwtPrintStyle style) {
            _type->print(*this, stream, style);
        }

        void GwtObject::addValue(std::string name, std::shared_ptr<GwtValue> value) {
            _values[name] = value;
        }

        std::map<std::string, std::shared_ptr<GwtValue>> GwtObject::values() const {
            return _values;
        }

        std::shared_ptr<GwtValue> GwtObject::value(const std::string &name) {
            return _values[name];
        }
    }
}

