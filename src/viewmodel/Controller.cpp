//
// Created by alexx on 27.04.2016.
//

#include "Controller.h"
#include <boost/filesystem/path.hpp>
#include <regex>
#include <QApplication>

namespace vantagefx {
    namespace viewmodel {
        using std::placeholders::_1;
        using std::placeholders::_2;
        using namespace api;
        namespace fs = boost::filesystem;

        Controller::Controller(GwtHttpContext &&context)
                : _context(std::move(context)),
                  _instrumentTypeId(0),
                  _brandId(0),
                  _loggedIn(false),
                  _money(-1),
				  _state(Idle) { }

        bool Controller::load() {
            using namespace api;
            using namespace http;

			if (!prepare()) return false;
            std::string url = "https://binaryoptions.vantagefx.com/app/index.html";
            HttpRequest index(url);
            _context.send(std::move(index), std::bind(&Controller::indexLoaded, this, _1, _2));
            return true;
        }

        bool Controller::auth(const std::string &login, const std::string &password, const std::string &server)
		{
            if (!prepare()) return false;
            _context.gwt(GwtAuthRequest(login, password, server), //"26251", "3361e6c1147f89e", "live"
                         std::bind(&Controller::loggedIn, this, _1, _2));
            return true;
        }

        bool Controller::refresh() {
			if (!prepare()) return false;
			_context.gwt(GwtCometUpdatesRequest(_instrumentTypeId),
                         std::bind(&Controller::refreshLoaded, this, _1, _2));
            return true;
        }

        bool Controller::buy(int64_t optionId, int64_t money, int positionType) {
			if (!prepare()) return false;
	        auto assetId = _refresh->item("options/[id = {0}]/assetId", { GwtValue(optionId) }).toInt();
			auto price = _refresh->item("options/[id = {0}]/price", { GwtValue(optionId) }).toDouble();
			_context.gwt(GwtPrepare2OpenPositionRequest(_accountId, optionId, assetId, money, price, positionType),
				std::bind(&Controller::positionOpened, this, _1, _2));
            return true;
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
                _rates[rateLut->value("name").toString()] = rateLut->value("id").toInt();
            }

            _servers.clear();
            for (auto &server: _lut->query("servers/*/key()")) {
                _servers.push_back(server.value.toString());
            }

            _context.gwt(GwtInstrumentConfigurationDataRequest(_brandId),
                         std::bind(&Controller::instrumentConfigurationLoaded, this, _1, _2));
        }

        void Controller::instrumentConfigurationLoaded(GwtObjectPtr &&instrumentConfiguration,
                                                       const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _instrumentConfiguration = std::move(instrumentConfiguration);
            _context.gwt(GwtInstrumentTypeIdsWithOpenOptionsRequest(),
                         std::bind(&Controller::instrumentOptionsLoaded, this, _1, _2));
        }

        void Controller::instrumentOptionsLoaded(GwtObjectPtr &&instrumentOptions,
                                                 const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _instrumentOptions = std::move(instrumentOptions);
            _context.gwt(GwtCometUpdatesRequest(_instrumentTypeId),
                         std::bind(&Controller::refreshLoaded, this, _1, _2));
        }

        void Controller::loggedIn(GwtObjectPtr &&auth, const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _auth = std::move(auth);
			_loggedIn = true;
            _fullName = _auth->item("account/fullName").toString();
            _accountId = _auth->item("account/accountId").toLong();
            _email = _auth->item("account/email").toString();
            _state = Ready;
        }

        void Controller::refreshLoaded(GwtObjectPtr &&data, const boost::optional<std::exception> &e) {
            if (handleError(e)) return;
            _refresh = std::move(data);

            _money = _refresh->item("money/value").toLong();

			for (auto &opt : _refresh->query("options/[optionStatus={0}]", { _openId })) {
				auto obj = opt.value.toObject();
				auto id = obj->value("id").toLong();
				auto &result = _options[id];
                loadOption(opt.value, result);
            }
            _state = Ready;
        }

		void Controller::positionOpened(GwtObjectPtr &&data, const boost::optional<std::exception> &e) {
			if (handleError(e)) return;
			_context.gwt(GwtOpenPositionRequest(data),
				std::bind(&Controller::buyComplete, this, _1, _2));
		}

		void Controller::buyComplete(GwtObjectPtr &&data, const boost::optional<std::exception> &e) {
			if (handleError(e)) return;
			_transactionId = data->value("transactionId").toLong();
			_state = Ready;
		}

	    int Controller::rateId(std::string name)
        {
			return _rates.at(name);
        }

	    bool Controller::handleError(const boost::system::error_code &ec) {
            if (!ec) return false;
            _e = boost::system::system_error(ec);
			_state = Ready;
			return true;
        }

		bool Controller::handleError(const boost::optional<std::exception> &e) {
			if (!e) return false;
			_e = e;
			_state = Ready;
			return true;
		}
		
		bool Controller::prepare()
	    {
		    auto check = Idle;
		    while(!_state.compare_exchange_strong(check, Busy)) {
				QCoreApplication::processEvents();
				check = Idle;
		    }
			return true;
	    }

	    void Controller::finish()
	    {
			_e.reset();
		    auto check = Ready;
		    if (!_state.compare_exchange_strong(check, Idle)) {
			    throw std::runtime_error("server is busy");
		    }
	    }

        void Controller::loadOption(const api::GwtValue &value, model::GwtOptionModel &model) {
            auto obj = value.toObject();
            auto id = obj->value("id").toInt();
            auto asset = obj->value("assetId");
            model.setOptionId(id);
            model.setAssetId(asset.toInt());
            model.setMoneyBack(obj->value("return").toInt());
            model.setSeconds(obj->value("optionSeconds").toInt());
            model.setReturnValue(obj->value("return").toInt());
            model.setName(_lut->item("assets/[id={0}]/name", { asset }).toString());
            model.setPrice(_refresh->item("assetUpdates/[assetId={0}]/targetPrice", { asset }).toDouble());
            auto subMarket = _lut->item("assets/[id={0}]/subMarketId", { asset });
            auto marketName = _lut->item("markets/[subMarkets/*/id = {0}]/name", { subMarket }).toString();
            auto marketId = _lut->item("markets/[subMarkets/*/id = {0}]/id", { subMarket }).toInt();
            auto subMarketName = _lut->item("markets/*/subMarkets/[id = {0}]/name", { subMarket }).toString();
            model.setMarketId(marketId);
            model.setMarket(marketName);
            model.setSubMarket(subMarketName);
            model.setClose(obj->item("closeDate/value").toLong());
            auto rates = _refresh->item("positionsSentimentDto/map/[int()={0}]", { asset }).toObject();
            if (rates) {
                for (auto &pair : _rates) {
					auto rate = rates->item("[int() = {0}]/value", { GwtValue(pair.second) });
                    if (!rate.empty()) {
                        model.setRate(pair.first, rate.toInt());
                    }
                }
            }
        }

        model::GwtOptionModel Controller::optionInfo(int64_t optionId)
        {
            model::GwtOptionModel result;
            prepare();
            try {
                loadOption(_refresh->item("options/[id={0}]", { GwtValue(optionId) }), result);
                _state = Idle;
            }
            catch(...)
            {
                _state = Idle;
                throw;
            }
            return result;
        }

		void Controller::save() const
        {
            _lut->saveXml(fs::path(DATA_DIR) / "lut.xml");
            _instrumentConfiguration->saveXml(fs::path(DATA_DIR) / "instrumentConfiguration.xml");
            _instrumentOptions->saveXml(fs::path(DATA_DIR) / "instrumentOptions.xml");
            _refresh->saveXml(fs::path(DATA_DIR) / "refresh.xml");
            _auth->saveXml(fs::path(DATA_DIR) / "auth.xml");
        }

        void Controller::stop() {
            _context.stop();
        }

	    void Controller::wait()
        {
			prepare();
			_state = Idle;
        }
    }
}