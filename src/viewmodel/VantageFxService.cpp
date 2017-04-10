//
// Created by alexx on 27.04.2016.
//

#include "VantageFxService.h"
#include <regex>
#include "../model/VantageFxModel.h"
#include <log4cplus/loggingmacros.h>

namespace vantagefx {
    namespace viewmodel {
        using std::placeholders::_1;
        using std::placeholders::_2;
        using namespace api;
        namespace fs = boost::filesystem;


        /*********************************************************************/
        /*                          LoadingContext                           */
        /*********************************************************************/

        LoadingContext::LoadingContext(GwtHttpContext &context, Handler handler, Fail error)
	    : _brandId(0),
	      _context(context),
	      _handler(handler),
	      _error(error) { }

        LoadingContext::~LoadingContext()
        {
        }

	    void LoadingContext::send()
        {
			auto self = shared_from_this();
			std::string url = "https://binaryoptions.vantagefx.com/";
			http::HttpRequest index(url);
			_context.send(std::move(index), std::bind(&LoadingContext::indexLoaded, self, _1, _2));
		}

	    void LoadingContext::indexLoaded(http::HttpResponse &&response, const boost::system::error_code &ec)
        {
            auto self = shared_from_this();
            if (ec) {
                std::stringstream stream;
                stream << ec;
                _error(boost::system::system_error(ec, stream.str()));
                return;
            }
            auto rx = std::regex("var\\s+(\\w+)\\s*=\\s*'([^']+)'\\s*;");
            std::smatch m;
            auto it = response.body().begin();
            while (std::regex_search(it, response.body().end(), m, rx)) {
                auto key = std::string(m[1].first, m[1].second);
                auto value = std::string(m[2].first, m[2].second);
                _keys[key] = value;
                it = m[0].second;
            }
            _context.gwt(GwtCookieRequest(_keys["serverCookie"]),
                         std::bind(&LoadingContext::cookieSets, self, _1, _2));
        }

        void LoadingContext::cookieSets(api::GwtObjectPtr &&cookie, const boost::optional<std::exception> &e)
        {
            auto self = shared_from_this();
            if (e) {
                _error(*e);
                return;
            }
            _context.gwt(GwtLutRequest(), std::bind(&LoadingContext::lutLoaded, self, _1, _2));
        }

        void LoadingContext::lutLoaded(api::GwtObjectPtr &&lut, const boost::optional<std::exception> &e)
        {
            auto self = shared_from_this();
            if (e) {
                _error(*e);
                return;
            }

            _lut = std::move(lut);

            _brandId = _lut->value("externalId").toInt();

            _context.gwt(GwtInstrumentConfigurationDataRequest(_brandId),
                         std::bind(&LoadingContext::instrumentConfigurationLoaded, self, _1, _2));
        }

        void LoadingContext::instrumentConfigurationLoaded(api::GwtObjectPtr &&instrumentConfiguration,
                                                           const boost::optional<std::exception> &e)
		{
            auto self = shared_from_this();
            if (e) {
                _error(*e);
                return;
            }
            _instrumentConfiguration = std::move(instrumentConfiguration);
            _context.gwt(GwtInstrumentTypeIdsWithOpenOptionsRequest(),
                         std::bind(&LoadingContext::instrumentOptionsLoaded, self, _1, _2));
        }

        void LoadingContext::instrumentOptionsLoaded(api::GwtObjectPtr &&instrumentOptions,
                                                     const boost::optional<std::exception> &e)
		{
            auto self = shared_from_this();
            if (e) {
                _error(*e);
                return;
            }
            _instrumentOptions = std::move(instrumentOptions);
            _handler(self);
        }


        /*********************************************************************/
        /*                          RefreshContext                           */
        /*********************************************************************/

        RefreshContext::RefreshContext(GwtHttpContext &context, Handler handler, Fail error)
                : _context(context),
                  _handler(handler),
                  _error(error)
        {
        }

        RefreshContext::~RefreshContext()
        {
        }

	    void RefreshContext::send(int instrumentTypeId)
        {
			auto self = shared_from_this();
			_context.gwt(GwtCometUpdatesRequest(instrumentTypeId),
				std::bind(&RefreshContext::refreshLoaded, self, _1, _2));
		}

	    void RefreshContext::refreshLoaded(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e) {
            auto self = shared_from_this();
            if (e) {
                _error(*e);
                return;
            }

            _refresh = std::move(data);
            _handler(self);
        }


        /*********************************************************************/
        /*                           AuthContext                            */
        /*********************************************************************/

        AuthContext::AuthContext(GwtHttpContext &context, Handler handler, Fail error)
                : _context(context),
                  _handler(handler),
                  _error(error)
        {
        }

        AuthContext::~AuthContext()
        {
        }

	    void AuthContext::send(const std::string& login, const std::string& password, const std::string& server)
        {
			auto self = shared_from_this();
			_context.gwt(GwtAuthRequest(login, password, server), std::bind(&AuthContext::loggedIn, self, _1, _2));
        }

	    void AuthContext::loggedIn(api::GwtObjectPtr &&auth, const boost::optional<std::exception> &e)
        {
            auto self = shared_from_this();
            if (e) {
                _error(*e);
                return;
            }
            _auth = std::move(auth);
            _handler(self);
        }


        /*********************************************************************/
        /*                         PurchaseContext                           */
        /*********************************************************************/

        PurchaseContext::PurchaseContext(GwtHttpContext &context, Handler handler, Fail error)
                : _context(context),
                  _handler(handler),
                  _error(error)
        {
        }

        PurchaseContext::~PurchaseContext()
        {
        }

	    void PurchaseContext::send(int64_t accountId, int64_t optionId, int assetId, 
			                       int64_t money, double price, int positionType)
        {
			auto self = shared_from_this();
			_context.gwt(GwtPrepare2OpenPositionRequest(accountId, optionId, assetId, money, price, positionType),
				std::bind(&PurchaseContext::positionOpened, self, _1, _2));
		}

	    void PurchaseContext::positionOpened(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e) {
            auto self = shared_from_this();
            if (e) {
                _error(*e);
                return;
            }
            _context.gwt(GwtOpenPositionRequest(data), std::bind(&PurchaseContext::buyComplete, self, _1, _2));
        }

        void PurchaseContext::buyComplete(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e) {
            auto self = shared_from_this();
            if (e) {
                _error(*e);
                return;
            }
			_transaction = std::move(data);
            _handler(self);
        }


        /*********************************************************************/
        /*                         VantageFxService                          */
        /*********************************************************************/

        VantageFxService::VantageFxService(GwtHttpContext &&context)
                : _logger(log4cplus::Logger::getInstance(L"VantageFxService")),
			      _context(std::move(context))
        {}

        LoadingContextPtr VantageFxService::load(LoadingContext::Handler handler, LoadingContext::Fail error)
        {
            auto result = std::make_shared<LoadingContext>(_context, handler, error);
			result->send();
			return result;
        }

        RefreshContextPtr VantageFxService::refresh(int instrumentTypeId, RefreshContext::Handler handler,
                                                    RefreshContext::Fail error)
        {
            auto result = std::make_shared<RefreshContext>(_context, handler, error);
			result->send(instrumentTypeId);
			return result;
        }

        AuthContextPtr VantageFxService::auth(const std::string &login, const std::string &password,
                                              const std::string &server, AuthContext::Handler handler,
                                              AuthContext::Fail error)
        {
			auto result = std::make_shared<AuthContext>(_context, handler, error);
			result->send(login, password, server);
			return result;
		}

        PurchaseContextPtr VantageFxService::buy(int64_t accountId, int64_t optionId, int assetId, int64_t money,
                                                 double price, int positionType, PurchaseContext::Handler handler,
                                                 PurchaseContext::Fail error)
        {
			auto msg = QString("buy(%1, %2, %3, %4, %L5, %6)").arg(accountId).arg(optionId).arg(assetId).arg(money).arg(price, 0, 'f').arg(positionType);
			LOG4CPLUS_INFO(_logger, msg.toStdWString());
			auto result = std::make_shared<PurchaseContext>(_context, handler, error);
			result->send(accountId, optionId, assetId, money, price, positionType);
			return result;
		}

        void VantageFxService::stop() {
            _context.stop();
        }
    }
}
