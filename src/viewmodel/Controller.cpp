//
// Created by alexx on 27.04.2016.
//

#include "Controller.h"
#include <boost/filesystem/path.hpp>
#include <regex>

namespace vantagefx {
    namespace viewmodel {
        using namespace api;
        namespace fs = boost::filesystem;

        Controller::Controller(GwtHttpContext &&context)
                : _context(std::move(context)),
                  _instrumentTypeId(0),
                  _brandId(0) { }

        void Controller::load(std::function<void(Controller &controller)> handler) {
            using namespace api;
            using namespace http;

            _handler = handler;
            std::string url = "https://binaryoptions.vantagefx.com/app/index.html";

            std::map<std::string, std::string> keys;

            HttpRequest index(url);
            _context.send(std::move(index), std::bind(&Controller::indexLoaded, this, _1, _2));
        }

        void Controller::auth(const std::string &login,
                              const std::string &password,
                              const std::string &server,
                              std::function<void(Controller &controller)> handler) {
            _handler = handler;
            _context.gwt(GwtAuthRequest(login, password, server), //"26251", "3361e6c1147f89e", "live"
                         std::bind(&Controller::loggedIn, this, _1, _2));
        }

        void Controller::refresh(std::function<void(Controller &controller)> handler) {
            _handler = handler;
            _context.gwt(GwtCometUpdatesRequest(_instrumentTypeId),
                         std::bind(&Controller::refreshLoaded, this, _1, _2));
        }

        void Controller::indexLoaded(http::HttpResponse && response, const boost::system::error_code &ec) {
            if (handleError(ec)) return;

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
                         std::bind(&Controller::cookieSets, this, _1, _2));
        }

        void Controller::cookieSets(GwtObjectPtr &&cookie, const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _context.gwt(GwtLutRequest(),
                         std::bind(&Controller::lutLoaded, this, _1, _2));
        }

        void Controller::lutLoaded(GwtObjectPtr &&lut, const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _lut = std::move(lut);

            auto instrumentTypeSuper = _lut->item("lutTypes/[name='InstrumentTypeSuper']/luts/[name='ShortTerm']/id");
            _instrumentTypeId = _lut->item("superRels/[instrumentTypeSuperId={0}]/instrumentTypeId",
                                           { instrumentTypeSuper }).toInt();

			_openId = _lut->item("lutTypes/[name='OptionStatus']/luts/[name='Open']/id");

            _brandId = _lut->value("externalId").toInt();

            for(auto &rate : _lut->query("lutTypes/[name='PositionType']/luts/*")) {
                auto rateLut = rate.value.toObject();
                _rates[rateLut->value("name").toString()] = rateLut->value("id").toString();
            }

            _context.gwt(GwtInstrumentConfigurationDataRequest(_brandId),
                         std::bind(&Controller::instrumentConfigurationLoaded, this, _1, _2));
        }

        void Controller::instrumentConfigurationLoaded(GwtObjectPtr &&instrumentConfiguration, const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _instrumentConfiguration = std::move(instrumentConfiguration);
            _context.gwt(GwtInstrumentTypeIdsWithOpenOptionsRequest(),
                         std::bind(&Controller::instrumentOptionsLoaded, this, _1, _2));
        }

        void Controller::instrumentOptionsLoaded(GwtObjectPtr &&instrumentOptions, const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _instrumentOptions = std::move(instrumentOptions);
            _handler(*this);
        }

        void Controller::loggedIn(GwtObjectPtr &&auth, const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _auth = std::move(auth);
            _handler(*this);
        }

        void Controller::refreshLoaded(GwtObjectPtr &&data, const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _refresh = std::move(data);

			for (auto &opt : _refresh->query("options/[optionStatus={0}]", { _openId })) {

                auto obj = opt.value.toObject();
                auto id = obj->value("id").toInt();
                auto asset = obj->value("assetId");
                auto &result = _options[id];
                result.setOptionId(id);
                result.setAssetId(asset.toInt());
                result.setMoneyBack(obj->value("return").toInt());
                result.setSeconds(obj->value("optionSeconds").toInt());
                result.setName(QString(_lut->item("assets/[id={0}]/name", { asset }).toString().c_str()));
                result.setPrice(_refresh->item("assetUpdates/[assetId={0}]/targetPrice", { asset }).toDouble());
                result.setClose(QDateTime::fromMSecsSinceEpoch(obj->item("closeDate/value").toLong()));
                auto rates = _refresh->item("positionsSentimentDto/map/[int()={0}]", { asset }).toObject();
				if (rates) {
					for (auto &pair : _rates) {
						if (rates->has(pair.second)) {
							result.setRate(QString(pair.first.c_str()), rates->item(pair.second + "/value").toInt());
						}
					}
				}
            }
            _handler(*this);
        }

        bool Controller::handleError(const boost::system::error_code &ec) {
            if (!ec) return false;
            _e = boost::system::system_error(ec);
            _handler(*this);
            return true;
        }

        bool Controller::handleError(const boost::optional<std::exception> &e) {
            if (!e) return false;
            _e = e;
            _handler(*this);
            return true;
        }

        void Controller::save() {
            _lut->saveXml(fs::path(DATA_DIR) / "lut.xml");
            _instrumentConfiguration->saveXml(fs::path(DATA_DIR) / "instrumentConfiguration.xml");
            _instrumentOptions->saveXml(fs::path(DATA_DIR) / "instrumentOptions.xml");
            _refresh->saveXml(fs::path(DATA_DIR) / "refresh.xml");
            _auth->saveXml(fs::path(DATA_DIR) / "auth.xml");
        }
    }
}