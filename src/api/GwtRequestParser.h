//
// Created by AMakarenya on 30.03.2016.
//

#ifndef VANTAGEFX_GWTREQUESTPARSER_H
#define VANTAGEFX_GWTREQUESTPARSER_H

#include <string>
#include "GwtResponseData.h"

namespace vantagefx {
    namespace api {
        GwtResponseData ParseRequest(std::string content);
    }
}


#endif //VANTAGEFX_GWTREQUESTPARSER_H
