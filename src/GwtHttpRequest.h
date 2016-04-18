//
// Created by AMakarenya on 18.04.2016.
//

#ifndef VANTAGEFX_GWTHTTPREQUEST_H
#define VANTAGEFX_GWTHTTPREQUEST_H

#include <string>
#include <src/http/HttpRequest.h>

namespace vantagefx {

    class GwtHttpRequest : public http::HttpRequest {
    public:
        GwtHttpRequest(std::string uri);
    };
}


#endif //VANTAGEFX_GWTHTTPREQUEST_H
