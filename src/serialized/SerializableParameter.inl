//
// Created by AMakarenya on 23.03.2016.
//

#include <boost/make_shared.hpp>
#include "SerializableParameter.h"
#include "IntegerParameter.h"
#include "StringParameter.h"
#include "ListParameter.h"
#include "MapParameter.h"
#include "GwtSerializableId.h"

namespace vantagefx {
    namespace api {
        using namespace serialized;

        template<>
        struct GwtReflectedType<SerializableParameter> {
            static std::string className() { return "net.sf.gilead.pojo.gwt.IGwtSerializableParameter"; }

            static void check(std::string name) {
                if (name != GwtReflectedType<IntegerParameter>::className()
                    && name != GwtReflectedType<StringParameter>::className()
                    && name != GwtReflectedType<ListParameter>::className()
                    && name != GwtReflectedType<MapParameter>::className()
                    && name != GwtReflectedType<GwtSerializableId>::className())
                    throw UnexpectedType(name, className());
            }

            static std::shared_ptr<SerializableParameter> create(int typeId, GwtParseContext &ctx) {
                auto typeName = ctx.word(typeId);

                if (typeName == GwtReflectedType<IntegerParameter>::className()) {
                    auto value = std::make_shared<IntegerParameter>();
                    ctx >> *value;
                    return value;
                }
                if (typeName == GwtReflectedType<StringParameter>::className()) {
                    auto value = std::make_shared<StringParameter>();
                    ctx >> *value;
                    return value;
                }
                if (typeName == GwtReflectedType<ListParameter>::className()) {
                    auto value = std::make_shared<ListParameter>();
                    ctx >> *value;
                    return value;
                }
                if (typeName == GwtReflectedType<MapParameter>::className()) {
                    auto value = std::make_shared<MapParameter>();
                    ctx >> *value;
                    return value;
                }
                if (typeName == GwtReflectedType<GwtSerializableId>::className()) {
                    auto value = std::make_shared<GwtSerializableId>();
                    ctx >> *value;
                    return value;
                }
                throw std::runtime_error("unexpected type: " + typeName + ", is not subclass of SerializableParameter");
            }
        };
    }
}
namespace std {
    inline ostream &operator<<(ostream &stream, const vantagefx::serialized::SerializableParameter &item) {
        return item.write(stream);
    }
}
