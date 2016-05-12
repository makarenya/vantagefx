//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_CONTROLLER_H
#define VANTAGEFX_CONTROLLER_H

#include <memory>
#include "../GwtHttpRequest.h"

namespace vantagefx {
    namespace model {
        class VantageFxModel;
    }
    namespace viewmodel {

        struct LoadingContext;
        typedef std::shared_ptr<LoadingContext> LoadingContextPtr;

        struct RefreshContext;
        typedef std::shared_ptr<RefreshContext> RefreshContextPtr;

        struct AuthContext;
        typedef std::shared_ptr<AuthContext> AuthContextPtr;

        struct PurchaseContext;
        typedef std::shared_ptr<PurchaseContext> PurchaseContextPtr;


        struct LoadingContext : public std::enable_shared_from_this<LoadingContext>
        {
            typedef std::function<void(LoadingContextPtr ctx)> Handler;
			typedef std::function<void(std::exception e)> Fail;

            LoadingContext(GwtHttpContext &context, Handler handler, Fail error);
			~LoadingContext();
			void send();

			api::GwtObjectPtr lut() const { return _lut; }

        private:

			std::map<std::string, std::string> _keys;
			api::GwtObjectPtr _lut;
			int _brandId;
			api::GwtObjectPtr _instrumentConfiguration;
			api::GwtObjectPtr _instrumentOptions;
			
			void indexLoaded(http::HttpResponse &&response, const boost::system::error_code &ec);

            void cookieSets(api::GwtObjectPtr &&cookie, const boost::optional<std::exception> &e);

            void lutLoaded(api::GwtObjectPtr &&lut, const boost::optional<std::exception> &e);

            void instrumentConfigurationLoaded(api::GwtObjectPtr &&instrumentConfiguration,
                                               const boost::optional<std::exception> &e);

            void instrumentOptionsLoaded(api::GwtObjectPtr &&instrumentOptions,
                                         const boost::optional<std::exception> &e);

            GwtHttpContext &_context;
            Handler _handler;
            Fail _error;
        };


        struct RefreshContext : public std::enable_shared_from_this<RefreshContext>
        {
            typedef std::function<void(RefreshContextPtr ctx)> Handler;
			typedef std::function<void(std::exception e)> Fail;
			
            RefreshContext(GwtHttpContext &context, Handler handler, Fail error);
			~RefreshContext();
			void send(int instrumentTypeId);

			api::GwtObjectPtr refresh() const { return _refresh; }

        private:

			api::GwtObjectPtr _refresh;
			
			void refreshLoaded(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e);

            GwtHttpContext &_context;
            Handler _handler;
            Fail _error;
        };

        struct AuthContext : public std::enable_shared_from_this<AuthContext>
        {
            typedef std::function<void(AuthContextPtr ctx)> Handler;
            typedef std::function<void(std::exception e)> Fail;

            AuthContext(GwtHttpContext &context, Handler handler, Fail error);
			~AuthContext();
			void send(const std::string &login, const std::string &password, const std::string &server);

			api::GwtObjectPtr auth() const { return _auth; };
		
		private:

			api::GwtObjectPtr _auth;
			
			void loggedIn(api::GwtObjectPtr &&auth, const boost::optional<std::exception> &e);

            GwtHttpContext &_context;
            Handler _handler;
            Fail _error;
        };


        struct PurchaseContext : public std::enable_shared_from_this<PurchaseContext> {
            typedef std::function<void(PurchaseContextPtr ctx)> Handler;
			typedef std::function<void(std::exception e)> Fail;

            PurchaseContext(GwtHttpContext &context, Handler handler, Fail error);
			~PurchaseContext();
			void send(int64_t accountId, int64_t optionId, int assetId, int64_t money, 
				      double price, int positionType);

			api::GwtObjectPtr transaction() const { return _transaction; }

        private:

			api::GwtObjectPtr _transaction;
			
			void positionOpened(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e);

            void buyComplete(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e);

            GwtHttpContext &_context;
            Handler _handler;
            Fail _error;
        };


        class VantageFxService
        {
        public:

            explicit VantageFxService(GwtHttpContext &&context);

            LoadingContextPtr load(LoadingContext::Handler handler, LoadingContext::Fail error);

            RefreshContextPtr refresh(int instrumentTypeId, RefreshContext::Handler handler, RefreshContext::Fail error);

            AuthContextPtr auth(const std::string &login, const std::string &password,
                                const std::string &server, AuthContext::Handler handler, AuthContext::Fail error);

            PurchaseContextPtr buy(int64_t accountId, int64_t optionId, int assetId, int64_t money, double price,
                                   int positionType, PurchaseContext::Handler handler, PurchaseContext::Fail error);

            void stop();

        private:

            GwtHttpContext _context;
        };
    }
}


#endif //VANTAGEFX_CONTROLLER_H
