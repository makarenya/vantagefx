//
// Created by alexx on 26.03.2016.
//

#include <iostream>
#include <fstream>
#include "GwtParser.h"
#include "GwtType.h"
#include "GwtBundle.h"
#include "GwtObject.h"

namespace vantagefx {
    namespace api {

	    GwtParser::GwtParser(StringList &&stringList, JsonVariantList &&data, GwtBundle &bundle)
                : GwtParseContext(std::move(stringList), std::move(data)),
                  _bundle(bundle) { }

        GwtObjectPtr GwtParser::parse(bool last) {
			if (eof()) return  GwtObjectPtr();

			int valueType = peekValueType();
			if (valueType >= 0) {
				GwtFieldPtr field;
				if (valueType == 0) field = fint("value");
				else if (valueType == 1) field = fdbl("value");
				else field = flng("value");
				auto value = popValue();
				std::vector<GwtFieldPtr> fields = { field };
				auto type = std::make_shared<GwtComplexType>("temp", fields, "value");
				auto obj = std::make_shared<GwtObject>(type);
				obj->values()["value"] = value;
				return obj;
			}

            int typeId;
            *this >> typeId;
            GwtObjectPtr obj;
            if (typeId == 0) {
                obj = GwtObjectPtr();
            }
            else if (typeId < 0) {
	            auto key = ~typeId;
                obj = _fetched[key];
            }
            else {
                auto it = _bundle.type(word(typeId));
                if (!it) {
                    throw ParseError("type " + word(typeId) + " not found");
                }
                obj = std::make_shared<GwtObject>(it);
                _fetched.push_back(obj);
				_fetchStack.push_back(obj);
                it->parse(*this, obj);
				_fetchStack.erase(_fetchStack.end() - 1);
            }
			if (last && !eof()) throw std::runtime_error("not all fetched");
            return obj;
        }

	    void GwtParser::printStack(std::ostream &stream)
	    {
		    for(auto &obj: _fetchStack) {
			    if (obj) {
					stream << obj->type()->name() << std::endl;
			    }
		    }
	    }

	    std::shared_ptr<GwtObject> GwtParser::root()
	    {
			if (_fetchStack.size() == 0) return GwtObjectPtr();
			return _fetchStack[0];
	    }

	    std::shared_ptr<GwtObject> GwtParser::last()
	    {
			for(int i = static_cast<int>(_fetchStack.size()) - 1; i >= 0; --i) {
				if (_fetchStack[i]) return _fetchStack[i];
			}
			return GwtObjectPtr();
	    }
    }
}