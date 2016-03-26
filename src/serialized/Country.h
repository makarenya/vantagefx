//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_COUNTRY_H
#define QT_SECOND_COUNTRY_H

#include "../api/GwtParseContext.h"


namespace vantagefx {
	namespace serialized {

		struct Country {
			std::string id;
			int some1;
			std::string language;
			int some2;
			int some3;
			std::string short2;
			std::string short3;
			std::string fullName;
			int some4;
			std::string someNumber;
			std::string fullName2;
		};
	}

	namespace api {
		template<>
		struct GwtReflectedType<serialized::Country>
		{
			static std::string className() { return "com.optionfair.client.model.Country"; }
			static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
		};

		GwtParseContext & operator>>(GwtParseContext &ctx, serialized::Country &result);
	}
}
namespace std {
	std::ostream & operator<<(std::ostream& stream, const vantagefx::serialized::Country &item);
}


#endif //QT_SECOND_COUNTRY_H
