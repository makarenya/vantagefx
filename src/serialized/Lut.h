//
// Created by AMakarenya on 22.03.2016.
//

#ifndef QT_SECOND_LUT_H
#define QT_SECOND_LUT_H

#include <src/api/GwtParseContext.h>


namespace vantagefx {
    namespace serialized {

        struct Lut {
            std::string parent;
            int order;
            std::string word;
            int some;
        };
    }

    namespace api {
		template<>
		struct GwtReflectedType<serialized::Lut>
		{
			static std::string className() { return "com.optionfair.client.model.Lut"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
		};
		
		GwtParseContext & operator>>(GwtParseContext &ctx, serialized::Lut &result);
    }
}
namespace std {
    ostream & operator<<(ostream& stream, const vantagefx::serialized::Lut &item);
}


#endif //QT_SECOND_LUT_H
