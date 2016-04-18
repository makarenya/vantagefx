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
                auto it = _bundle.type(typeName(typeId));
                if (!it) {
                    throw ParseError("type " + typeName(typeId) + " not found");
                }
                obj = std::make_shared<GwtObject>(it);
                _fetched.push_back(obj);
                auto oldObject = _currentObject;
                _currentObject = obj;
                it->parse(*this, obj);
                _currentObject = oldObject;
            }
			if (last && !eof()) throw std::runtime_error("not all fetched");
            return obj;
        }
    }
}