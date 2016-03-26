//
// Created by alexx on 21.03.2016.
//

#ifndef QT_SECOND_GWTRESPONSEDATA_H
#define QT_SECOND_GWTRESPONSEDATA_H

#include <string>
#include <vector>
#include <boost/variant.hpp>

namespace vantagefx {
	namespace api {

		typedef boost::variant<int, double, bool, std::string> JsonVariant;
		typedef std::vector<JsonVariant> JsonVariantList;
		typedef std::vector<std::string> StringList;

		struct GwtResponseData {
			int version = 0;
			int flags = 0;
			StringList strings;
			JsonVariantList data;
		};

		enum GwtFlags {
			None = 0,
			ElideTtypeNames = 1,
			RpcTokenIcluded = 2
		};

	}
}


#endif //QT_SECOND_GWTRESPONSEDATA_H
