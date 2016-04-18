//
// Created by alexx on 21.03.2016.
//

#ifndef QT_SECOND_RESPONSEPARSER_H
#define QT_SECOND_RESPONSEPARSER_H

#include "GwtParser.h"

namespace vantagefx {
    namespace api {
	    class GwtBundle;
	    GwtParser makeResponseParser(std::string respons, GwtBundle &bundlee);
    }
}

#endif //QT_SECOND_RESPONSEPARSER_H
