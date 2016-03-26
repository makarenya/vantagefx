//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_INSTRUMENTTYPEFILTER_H
#define QT_SECOND_INSTRUMENTTYPEFILTER_H

#include "../api/GwtParseContext.h"


namespace vantagefx {
	namespace serialized {

		struct InstrumentTypeFilter {
			std::string serialuid;
			int some1;
			std::string interval;
			int order;
			int some2;
			std::string title;
		};
	}

	namespace api {
		template<>
		struct GwtReflectedType<serialized::InstrumentTypeFilter>
		{
			static std::string className() { return "com.optionfair.client.model.InstrumentTypeFilter"; }
			static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
		};

		GwtParseContext & operator>>(GwtParseContext &ctx, serialized::InstrumentTypeFilter &result);
	}
}
namespace std {
	ostream & operator<<(ostream& stream, const vantagefx::serialized::InstrumentTypeFilter &item);
}


#endif //QT_SECOND_INSTRUMENTTYPEFILTER_H
