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
        using std::placeholders::_1;
        using std::placeholders::_2;

        class Controller
        {
        public:

            explicit Controller(GwtHttpContext &&context);

            void load(std::function<void(Controller &controller)> handler);

            void auth(const std::string &login,
                      const std::string &password,
                      const std::string &server,
                      std::function<void(Controller &controller)> handler);

            void refresh(std::function<void(Controller &controller)> handler);

            void save();

            bool is_successfull() const { return !!_e; }
            const std::exception &exception() const { return *_e; }
            QMap<int, model::GwtOptionModel> options() const { return _options; }

        private:

            bool handleError(const boost::optional<std::exception> &e);

            bool handleError(const boost::system::error_code &ec);

            void indexLoaded(http::HttpResponse && response, const boost::system::error_code &ec);

            void cookieSets(api::GwtObjectPtr &&cookie, const boost::optional<std::exception> &e);

            void lutLoaded(api::GwtObjectPtr &&lut, const boost::optional<std::exception> &e);

            void instrumentConfigurationLoaded(api::GwtObjectPtr &&instrumentConfiguration,
                                               const boost::optional<std::exception> &e);

            void instrumentOptionsLoaded(api::GwtObjectPtr &&instrumentOptions,
                                         const boost::optional<std::exception> &e);

            void loggedIn(api::GwtObjectPtr &&auth, const boost::optional<std::exception> &e);

            void refreshLoaded(api::GwtObjectPtr &&data, const boost::optional<std::exception> &e);

        private:
            GwtHttpContext _context;
            api::GwtObjectPtr _lut;
            int _instrumentTypeId;
            int _brandId;
            std::map<std::string, std::string> _rates;
            api::GwtObjectPtr _instrumentConfiguration;
            api::GwtObjectPtr _instrumentOptions;
            api::GwtObjectPtr _refresh;
            api::GwtObjectPtr _auth;
            std::map<std::string, std::string> _keys;
            QMap<int, model::GwtOptionModel> _options;
            boost::optional<std::exception> _e;
            std::function<void(Controller &controller)> _handler;
        };
    }
}


#endif //VANTAGEFX_CONTROLLER_H
