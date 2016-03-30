//
// Created by AMakarenya on 23.03.2016.
//

#ifndef QT_SECOND_SERIALIZABLEPARAMETER_H
#define QT_SECOND_SERIALIZABLEPARAMETER_H

#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "../api/GwtParseContext.h"

namespace vantagefx {
    namespace serialized {
        struct SerializableParameter {
            virtual ~SerializableParameter() { }

            virtual std::ostream &write(std::ostream &stream) const = 0;
        };

        typedef std::shared_ptr<SerializableParameter> SerializableParameterPtr;
    }
}
namespace std {
    std::ostream &operator<<(std::ostream &stream, const vantagefx::serialized::SerializableParameter &item);
}

#endif //QT_SECOND_SERIALIZABLEPARAMETER_H
