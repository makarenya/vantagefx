//
// Created by AMakarenya on 18.04.2016.
//

#include "GwtHttpRequest.h"
#include <boost/lexical_cast.hpp>

namespace vantagefx {

	using boost::system::error_code;

	GwtHttpContext::GwtHttpContext(boost::asio::io_service& io_service, boost::asio::ssl::context& context, api::GwtBundle& bundle)
		: HttpContext(io_service, context), 
		  _bundle(bundle)
	{}


	void GwtHttpContext::gwt(vantagefx::GwtHttpRequest&& request, const GwtHandler &handler)
	{
		api::GwtObjectPtr result;
		send(std::move(request), [this, handler](http::HttpResponse &&response, const error_code &ec)
		{
			if (ec) {
				handler(api::GwtObjectPtr(), boost::system::system_error(ec));
				return;
			}
			auto parser = api::makeResponseParser(response.body(), _bundle);

			try {
				handler(parser.parse(), boost::optional<std::exception>());
			}
			catch (std::exception &e) {
				std::cout << e.what() << std::endl;
				parser.back(10);
				parser.print(std::cout, 9);
				std::cout << ">>> ";
				parser.print(std::cout, 100);
				handler(api::GwtObjectPtr(), e);
			}
		});
	}

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

	GwtInstrumentConfigurationDataRequest::GwtInstrumentConfigurationDataRequest(int externalId)
		: GwtHttpRequest("Lut")
	{
		std::string body = "7|0|5|https://binaryoptions.vantagefx.com/app/Basic/|F5609C0B32D4C72047639979C6C333F4|";
		body += "com.optionfair.client.common.services.LutService|getInstrumentConfigurationData|I|1|2|3|4|1|5|";
		body += boost::lexical_cast<std::string>(externalId) + "|";
		setContent(body);
    }

	GwtInstrumentTypeIdsWithOpenOptionsRequest::GwtInstrumentTypeIdsWithOpenOptionsRequest()
		: GwtHttpRequest("Trading")
	{
	    std::string body = "7|0|4|https://binaryoptions.vantagefx.com/app/Basic/|094DCA70134E3C91D91952E12643E178|";
		body += "com.optionfair.client.common.services.TradingService|getInstrumentTypeIdsWithOpenOptions|1|2|3|4|0|";
		setContent(body);
    }

	GwtCometUpdatesRequest::GwtCometUpdatesRequest(int instrumentTypeId)
		: GwtHttpRequest("refresh")
	{
		std::string body = "7|0|6|https://binaryoptions.vantagefx.com/app/Basic/|36F1204346683C3C0438E630893A8A1D|";
		body += "com.optionfair.client.common.services.nongenerated.RefreshService|getCometUpdates|I|J|1|2|3|4|3|5|6|6|";
		body += boost::lexical_cast<std::string>(instrumentTypeId) + "|A|A|";
		setContent(body);
	}
}
