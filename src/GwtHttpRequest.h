//
// Created by AMakarenya on 18.04.2016.
//

#ifndef VANTAGEFX_GWTHTTPREQUEST_H
#define VANTAGEFX_GWTHTTPREQUEST_H

#include <string>
#include <src/http/HttpRequest.h>
#include "http/HttpContext.h"
#include "api/GwtBundle.h"
#include "api/GwtResponseParser.h"

namespace vantagefx {

	class GwtHttpRequest;

	class GwtHttpContext : public http::HttpContext {
	public:
		GwtHttpContext(boost::asio::io_service& io_service, boost::asio::ssl::context& context, api::GwtBundle& bundle);

		api::GwtObjectPtr gwt(GwtHttpRequest&& request);

	private:
		api::GwtBundle &_bundle;
	};
	
	class GwtHttpRequest : public http::HttpRequest {
    public:
	    explicit GwtHttpRequest(const std::string &uri);
    };

    class GwtAuthRequest : public GwtHttpRequest {
    public:
        GwtAuthRequest(const std::string &login, const std::string &password, const std::string &server);
    };

    class GwtCookieRequest : public GwtHttpRequest {
    public:
	    explicit GwtCookieRequest(const std::string &serverCookie);
    };

    class GwtLutRequest : public GwtHttpRequest {
    public:
        GwtLutRequest();
    };

	class GwtInstrumentConfigurationDataRequest : public GwtHttpRequest {
	public:
		explicit GwtInstrumentConfigurationDataRequest(int externalId);
	};

	class GwtInstrumentTypeIdsWithOpenOptionsRequest : public GwtHttpRequest {
	public:
		GwtInstrumentTypeIdsWithOpenOptionsRequest();
	};

	class GwtCometUpdatesRequest : public GwtHttpRequest {
	public:
		explicit GwtCometUpdatesRequest(int instrumentTypeId);
	};
}


#endif //VANTAGEFX_GWTHTTPREQUEST_H
