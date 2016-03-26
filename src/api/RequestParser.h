//
// Created by alexx on 21.03.2016.
//

#ifndef QT_SECOND_REQUESTPARSER_H
#define QT_SECOND_REQUESTPARSER_H

#include "GwtResponseData.h"


namespace vantagefx {
    namespace api {

        JsonVariantList ParseRequest(std::string request);
    }
}

#endif //QT_SECOND_REQUESTPARSER_H
