//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTPARSER_H
#define VANTAGEFX_GWTPARSER_H

#include <map>
#include <stack>
#include "GwtResponseData.h"
#include "GwtParseContext.h"


namespace vantagefx {
    namespace api {

        class GwtType;

        class GwtObject;

        class GwtBundle;

        class GwtParser : public GwtParseContext {
        public:
            GwtParser(StringList &&stringList, JsonVariantList &&data, GwtBundle &bundle);

            std::shared_ptr<GwtObject> parse(bool last = false);

            std::shared_ptr<GwtObject> currentObject() const;
        private:
            GwtBundle &_bundle;
            std::vector<std::shared_ptr<GwtObject>> _fetched;
            std::shared_ptr<GwtObject> _currentObject;
        };

        inline std::shared_ptr<GwtObject> GwtParser::currentObject() const { return _currentObject; }
    }
}


#endif //VANTAGEFX_GWTPARSER_H
