//
// Created by AMakarenya on 30.03.2016.
//

#ifndef VANTAGEFX_GWTREQUESTPARSER_H
#define VANTAGEFX_GWTREQUESTPARSER_H

#include <string>
#include "GwtParser.h"

namespace vantagefx {
    namespace api {
        GwtParser makeRequestParser(std::string content, GwtBundle &bundle);
    }
}


#endif //VANTAGEFX_GWTREQUESTPARSER_H
