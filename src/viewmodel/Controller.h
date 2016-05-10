//
// Created by alexx on 27.04.2016.
//

#ifndef VANTAGEFX_CONTROLLER_H
#define VANTAGEFX_CONTROLLER_H

#include <memory>
#include "src/GwtHttpRequest.h"
#include "src/model/GwtOptionModel.h"
#include <QtCore>

namespace vantagefx {
    namespace viewmodel {

        class Controller
        {
        public:

            enum State {
                Idle,
                Busy,
                Ready
            };

            explicit Controller(GwtHttpContext &&context);

            bool load();

            bool auth(const std::string &login, const std::string &password, const std::string &server);

            bool refresh();

            bool buy(int64_t optionId, int64_t money, int positionType);

            void save() const;

            bool isReady() const { return _state == Ready && !_e; }

			bool isError() const { return _state == Ready && !!_e; }

			model::GwtOptionModel optionInfo(int64_t optionId);

            const std::exception &exception() const { return *_e; }

	        bool prepare();

	        void finish();

	        const std::map<int64_t, model::GwtOptionModel> &options() const { return _options; }
            const std::vector<std::string> &servers() const { return _servers; }
			bool isLoggedIn() const { return _loggedIn; }
            const std::string &fullName() const { return _fullName; }
            int64_t accountId() const { return _accountId; }
            const std::string &email() const { return _email; }
            int64_t money() const { return _money; }
			int rateId(std::string name);

            void stop();
	        void wait();
        private:

            void loadOption(const api::GwtValue &value, model::GwtOptionModel &model);

            bool handleError(const boost::optional<std::exception> &e);

            bool handleError(const boost::system::error_code &ec);

            void indexLoaded(http::HttpResponse && response, const boost::system::error_code &ec);

            void cookieSets(api::GwtObjectPtr &&cookie, const boost::optional<std::exception> &e);

            void lutLoaded(api::GwtObjectPtr &&lut, const boost::optional<std::exception> &e);

            void instrumentConfigurationLoaded(api::GwtObjectPtr &&instrumentConfiguration, const boost::optional<std::exception> &e);

            void instrumentOptionsLoaded(api::GwtObjectPtr &&instrumentOptions, const boost::optional<std::exception> &e);

            void loggedIn(api::GwtObjectPtr &&auth, const boost::optional<std::exception> &e);

            void refreshLoaded(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e);

			void positionOpened(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e);

			void buyComplete(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e);

        private:
            GwtHttpContext _context;
            api::GwtObjectPtr _lut;
            int _instrumentTypeId;
            int _brandId;
			api::GwtValue _openId;
            std::map<std::string, int> _rates;
            api::GwtObjectPtr _instrumentConfiguration;
            api::GwtObjectPtr _instrumentOptions;
            api::GwtObjectPtr _refresh;
            api::GwtObjectPtr _auth;
			bool _loggedIn;
            std::string _fullName;
            int64_t _accountId;
            std::string _email;
            int64_t _money;
			int64_t _transactionId;
            std::map<std::string, std::string> _keys;
            std::map<int64_t, model::GwtOptionModel> _options;
            std::vector<std::string> _servers;
            boost::optional<std::exception> _e;
            std::atomic<State> _state;
        };
    }
}


#endif //VANTAGEFX_CONTROLLER_H
