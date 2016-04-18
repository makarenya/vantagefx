//
// Created by AMakarenya on 18.04.2016.
//

#include "GwtHttpRequest.h"

namespace vantagefx {

    GwtHttpRequest::GwtHttpRequest(std::string uri)
        : http::HttpRequest("https://binaryoptions.vantagefx.com/app/" + uri, "POST")
    {
        addHeader("X-GWT-Module-Base", "https://binaryoptions.vantagefx.com/app/Basic/");
        addHeader("X-GWT-Permutation", "97A60A56BD971D60069316AF35BCDFB9");
        addHeader("Origin", "https://binaryoptions.vantagefx.com");
        addHeader("Content-Type", "text/x-gwt-rpc; charset=UTF-8");
    }
}

