//
// Created by AMakarenya on 18.04.2016.
//

#include "GwtHttpRequest.h"

namespace vantagefx {

    GwtHttpRequest::GwtHttpRequest(const std::string &uri)
            : http::HttpRequest("https://binaryoptions.vantagefx.com/app/services/" + uri, "POST")
	{
        addHeader("X-GWT-Module-Base", "https://binaryoptions.vantagefx.com/app/Basic/");
        addHeader("X-GWT-Permutation", "97A60A56BD971D60069316AF35BCDFB9");
        addHeader("Origin", "https://binaryoptions.vantagefx.com");
        addHeader("Content-Type", "text/x-gwt-rpc; charset=UTF-8");
    }

    GwtAuthRequest::GwtAuthRequest(const std::string &login, const std::string &password, const std::string &server)
            : GwtHttpRequest("AccountService")
    {
        std::string body = "7|0|8|https://binaryoptions.vantagefx.com/app/Basic/|413A5B61C75C56589A1EA33BAB70D8D3|";
        body += "com.optionfair.client.common.services.AccountService|login|java.lang.String/2004016611|";
        body += login + "|";
        body += password + "|";
        body += server + "|1|2|3|4|3|5|5|5|6|7|8|";

        setContent(body);
    }

    GwtCookieRequest::GwtCookieRequest(const std::string &serverCookie)
            : GwtHttpRequest("cookie") 
	{
		std::string body = "7|0|6|https://binaryoptions.vantagefx.com/app/Basic/|C4BF0ED8850B9A3E37D530BBD81A4817|";
		body += "com.optionfair.client.cookies.CookieSaver|saveCookie|java.lang.String/2004016611|";
		body += serverCookie + "|1|2|3|4|1|5|6|";
        
		setContent(body);
    }

    GwtLutRequest::GwtLutRequest()
            : GwtHttpRequest("Lut")
	{
        setContent("7|0|4|https://binaryoptions.vantagefx.com/app/Basic/|F5609C0B32D4C72047639979C6C333F4|"
                           "com.optionfair.client.common.services.LutService|getAll|1|2|3|4|0|");
    }
}
