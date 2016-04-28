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
#include "api/GwtHttpRequestContext.h"

namespace vantagefx {

	class GwtHttpRequest;

	class GwtHttpContext : public http::HttpContext {
	public:
		typedef std::function<void(api::GwtObjectPtr &&item, const boost::optional<std::exception> &e)> GwtHandler;

		GwtHttpContext(boost::asio::io_service& io_service, boost::asio::ssl::context& context, api::GwtBundle& bundle);

		void gwt(GwtHttpRequest&& request, const GwtHandler &handler);

	private:
		api::GwtBundle &_bundle;
	};


	class GwtRequestField
	{
	public:
		virtual ~GwtRequestField() {}
		virtual std::string type() const = 0;
		virtual void output(api::GwtHttpRequestContext &ctx) const = 0;
	};

	typedef std::shared_ptr<GwtRequestField> GwtRequestFieldPtr;

	class GwtRequestIntField : public GwtRequestField
	{
	public:
		explicit GwtRequestIntField(int value);
		std::string type() const override;
		void output(api::GwtHttpRequestContext &ctx) const override;
	private:
		int _value;
	};

	class GwtRequestDoubleField : public GwtRequestField
	{
	public:
		explicit GwtRequestDoubleField(double value);
		std::string type() const override;
		void output(api::GwtHttpRequestContext &ctx) const override;
	private:
		double _value;
	};

	class GwtRequestLongField : public GwtRequestField
	{
	public:
		explicit GwtRequestLongField(int64_t value);
		std::string type() const override;
		void output(api::GwtHttpRequestContext &ctx) const override;
	private:
		int64_t _value;
	};

	class GwtRequestStringField : public GwtRequestField
	{
	public:
		explicit GwtRequestStringField(std::string value);
		std::string type() const override;
		void output(api::GwtHttpRequestContext &ctx) const override;
	private:
		std::string _value;
	};

	class GwtRequestObjectField : public GwtRequestField
	{
	public:
		explicit GwtRequestObjectField(api::GwtObjectPtr value);
		std::string type() const override;
		void output(api::GwtHttpRequestContext &ctx) const override;
	private:
		api::GwtObjectPtr _value;
	};
	
	class GwtHttpRequest : public http::HttpRequest {
    public:
		explicit GwtHttpRequest(const std::string &uri, const std::string &uid,
			const std::string &servlet, const std::string &method);

		void updateContent();

		void intField(int value);

		void dblField(double value);

		void lngField(int64_t value);

		void strField(std::string value);

		void ptrField(api::GwtObjectPtr value);

	private:
        std::string _uid;
        std::string _servlet;
        std::string _method;
		std::vector<GwtRequestFieldPtr> _fields;
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
		explicit GwtCometUpdatesRequest(int instrumentTypeId, int64_t lastUpdated = 0, int64_t currentOption = 0);
	};

	class GwtPrepare2OpenPositionRequest : public GwtHttpRequest
	{
	public:
		explicit GwtPrepare2OpenPositionRequest(int64_t accountId, int64_t optionId, int64_t sum, double price, int positionType);
	};
}


#endif //VANTAGEFX_GWTHTTPREQUEST_H
