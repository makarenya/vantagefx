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

			std::vector<std::shared_ptr<GwtObject>> stack() const;
			void printStack(std::ostream &stream);
			std::shared_ptr<GwtObject> root();
			std::shared_ptr<GwtObject> last();
			void pushItem(std::string item) { _itemStack.push_back(item); }
			void popItem() { _itemStack.pop_back(); }
			void printItems(std::ostream &stream);
        private:
            GwtBundle &_bundle;
            std::vector<std::shared_ptr<GwtObject>> _fetched;
            std::vector<std::shared_ptr<GwtObject>> _fetchStack;
			std::vector<std::string> _itemStack;
        };

        inline std::vector<std::shared_ptr<GwtObject>> GwtParser::stack() const { return _fetchStack; }
    }
}


#endif //VANTAGEFX_GWTPARSER_H
