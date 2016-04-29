//
// Created by AMakarenya on 18.04.2016.
//

#include "GwtHttpRequest.h"
#include <boost/lexical_cast.hpp>

namespace vantagefx {
	using namespace api;

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

	GwtRequestIntField::GwtRequestIntField(int value) : _value(value) {}
	std::string GwtRequestIntField::type() const { return "I"; }
	void GwtRequestIntField::output(GwtHttpRequestContext &ctx) const { ctx << _value; }

	GwtRequestDoubleField::GwtRequestDoubleField(double value) : _value(value) {}
	std::string GwtRequestDoubleField::type() const { return "D"; }
	void GwtRequestDoubleField::output(GwtHttpRequestContext &ctx) const { ctx << _value; }

	GwtRequestLongField::GwtRequestLongField(int64_t value) : _value(value) {}
	std::string GwtRequestLongField::type() const { return "J"; }
	void GwtRequestLongField::output(GwtHttpRequestContext &ctx) const { ctx << _value; }

	GwtRequestStringField::GwtRequestStringField(std::string value) : _value(value) {}
	std::string GwtRequestStringField::type() const { return "java.lang.String/2004016611"; }
	void GwtRequestStringField::output(GwtHttpRequestContext &ctx) const { ctx << _value; }

	GwtRequestObjectField::GwtRequestObjectField(api::GwtObjectPtr value) : _value(value) {}
	std::string GwtRequestObjectField::type() const { return _value->type()->name(); }
	void GwtRequestObjectField::output(GwtHttpRequestContext &ctx) const { _value->serialize(ctx); }

	GwtHttpRequest::GwtHttpRequest(const std::string &uri, const std::string &uid,
		const std::string &servlet, const std::string &method)
            : http::HttpRequest("https://binaryoptions.vantagefx.com/app/services/" + uri, "POST"),
              _uid(uid),
              _servlet(servlet),
              _method(method)
	{
        addHeader("X-GWT-Module-Base", "https://binaryoptions.vantagefx.com/app/Basic/");
        addHeader("X-GWT-Permutation", "97A60A56BD971D60069316AF35BCDFB9");
        addHeader("Origin", "https://binaryoptions.vantagefx.com");
        addHeader("Content-Type", "text/x-gwt-rpc; charset=UTF-8");
    }

    void GwtHttpRequest::updateContent()
    {
        GwtHttpRequestContext ctx;
        ctx << "https://binaryoptions.vantagefx.com/app/Basic/";
        ctx << _uid;
        ctx << _servlet;
        ctx << _method;
        ctx << static_cast<int>(_fields.size());
        for(auto &prop : _fields) {
            ctx << prop->type();
        }
        for (auto &prop : _fields) {
            prop->output(ctx);
        }
        setContent(ctx.str());
    }

	void GwtHttpRequest::intField(int value)
	{
		_fields.push_back(std::make_shared<GwtRequestIntField>(value));
	}

	void GwtHttpRequest::dblField(double value)
	{
		_fields.push_back(std::make_shared<GwtRequestDoubleField>(value));
	}

	void GwtHttpRequest::lngField(int64_t value)
	{
		_fields.push_back(std::make_shared<GwtRequestLongField>(value));
	}

	void GwtHttpRequest::strField(std::string value)
	{
		_fields.push_back(std::make_shared<GwtRequestStringField>(value));
	}

	void GwtHttpRequest::ptrField(api::GwtObjectPtr value)
	{
		_fields.push_back(std::make_shared<GwtRequestObjectField>(value));
	}

	GwtAuthRequest::GwtAuthRequest(const std::string &login, const std::string &password, const std::string &server)
            : GwtHttpRequest("AccountService", "413A5B61C75C56589A1EA33BAB70D8D3", 
				"com.optionfair.client.common.services.AccountService", "login")
    {
        strField(login);
        strField(password);
		updateContent();
	}

    GwtCookieRequest::GwtCookieRequest(const std::string &serverCookie)
            : GwtHttpRequest("cookie", "C4BF0ED8850B9A3E37D530BBD81A4817", 
				"com.optionfair.client.cookies.CookieSaver", "saveCookie")
    {
        strField(serverCookie);
		updateContent();
	}

	GwtLutRequest::GwtLutRequest()
		: GwtHttpRequest("Lut", "F5609C0B32D4C72047639979C6C333F4",
			"com.optionfair.client.common.services.LutService", "getAll")
	{
		updateContent();
	}

	GwtInstrumentConfigurationDataRequest::GwtInstrumentConfigurationDataRequest(int externalId)
		: GwtHttpRequest("Lut", "F5609C0B32D4C72047639979C6C333F4",
			"com.optionfair.client.common.services.LutService", "getInstrumentConfigurationData")
    {
        intField(externalId);
		updateContent();
	}

	GwtInstrumentTypeIdsWithOpenOptionsRequest::GwtInstrumentTypeIdsWithOpenOptionsRequest()
		: GwtHttpRequest("Trading", "094DCA70134E3C91D91952E12643E178",
			"com.optionfair.client.common.services.TradingService", "getInstrumentTypeIdsWithOpenOptions")
	{
		updateContent();
	}

	GwtCometUpdatesRequest::GwtCometUpdatesRequest(int instrumentTypeId, int64_t lastUpdated, int64_t currentOption)
		: GwtHttpRequest("refresh", "36F1204346683C3C0438E630893A8A1D",
			"com.optionfair.client.common.services.nongenerated.RefreshService", "getCometUpdates")
    {
        intField(instrumentTypeId);
        lngField(lastUpdated);
        lngField(currentOption);
		updateContent();
	}

	GwtPrepare2OpenPositionRequest::GwtPrepare2OpenPositionRequest(int64_t accountId, int64_t optionId, int64_t sum, double price, int positionType)
		: GwtHttpRequest("Trading", "094DCA70134E3C91D91952E12643E178",
			"com.optionfair.client.common.services.TradingService", "prepare2OpenPosition")
    {
        lngField(accountId);
        lngField(optionId);
        lngField(5);
        lngField(sum);
        dblField(price);
        dblField(price);
        strField(boost::lexical_cast<std::string>(price));
        strField("");
        intField(positionType);
        strField("0");
        intField(1);
		updateContent();
    }

	GwtOpenPositionRequest::GwtOpenPositionRequest(const GwtObjectPtr &prepareResponse)
			: GwtHttpRequest("Trading", "094DCA70134E3C91D91952E12643E178",
				"com.optionfair.client.common.services.TradingService","openPosition")
	{
		ptrField(prepareResponse);
		updateContent();
	}
}


