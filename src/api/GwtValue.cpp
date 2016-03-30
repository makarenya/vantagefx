//
// Created by alexx on 26.03.2016.
//

#include "GwtValue.h"
#include "GwtParser.h"
#include "GwtObject.h"

namespace vantagefx {
    namespace api {

        int GwtValue::intValue() const {
            return boost::get<int>(_value);
        }

        double GwtValue::doubleValue() const {
            return boost::get<double>(_value);
        }

        int64_t GwtValue::longValue() const {
            return boost::get<int64_t>(_value);
        }

        std::shared_ptr<GwtObject> &GwtValue::objectValue() {
            return boost::get<std::shared_ptr<GwtObject>>(_value);
        }

        const std::string &GwtValue::stringValue() const {
            return _string;
        }
    }
}