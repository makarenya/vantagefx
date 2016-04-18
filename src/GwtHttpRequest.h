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
	    explicit GwtHttpRequest(const std::string &uri);
    };

    class GwtCookieRequest : public GwtHttpRequest {
    public:
	    explicit GwtCookieRequest(const std::string &serverCookie);
    };

    class GwtLutRequest : public GwtHttpRequest {
    public:
        GwtLutRequest();
    };
}


#endif //VANTAGEFX_GWTHTTPREQUEST_H
