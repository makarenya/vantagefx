//
// Created by alexx on 27.04.2016.
//

#include "OptionModel.h"
#include "TransactionModel.h"
#include <log4cplus/loggingmacros.h>

namespace vantagefx {
    namespace model {

	    OptionModel::OptionModel()
				: _logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("OptionModel"))),
	              _optionId(0),
	              _asset(nullptr),
	              _seconds(0),
			      _price(0),
			      _status(Idle),
			      _openTime()
			{}

	    int OptionModel::rate(const QString &name) const
	    {
			return asset().rateValue(name);
		}

	    void OptionModel::setOptionId(int64_t optionId)
		{
            _optionId = optionId;
        }

        void OptionModel::setAsset(AssetModel *asset)
		{
            _asset = asset;
        }

        void OptionModel::setSeconds(int seconds)
		{
            _seconds = seconds;
        }

        void OptionModel::setPrice(double price)
		{
            _price = price;
        }

        void OptionModel::setClose(QDateTime close)
		{
            _close = close;
        }

		void OptionModel::openTransaction()
	    {
			_status = Processing;
			_openTime = QDateTime::currentDateTime().addSecs(seconds() + 10);
	    }

	    void OptionModel::closeSuccess()
	    {
			_status = Successful;
			auto time = QDateTime::currentDateTime().addSecs(10);
			if (time > _openTime) _openTime = time;
			else if (time.secsTo(_openTime) > 5) {
				LOG4CPLUS_ERROR(_logger, LOG4CPLUS_TEXT("something wrong on close success"));
			}
	    }

	    void OptionModel::closeFail()
	    {
			_status = Failed;
			auto time = QDateTime::currentDateTime().addSecs(10);
			if (time > _openTime) _openTime = time;
			else if (time.secsTo(_openTime) > 5) {
				LOG4CPLUS_ERROR(_logger, LOG4CPLUS_TEXT("something wrong on close fail"));
			}
		}

	    void OptionModel::closeReturn()
	    {
			_status = Returned;
			auto time = QDateTime::currentDateTime().addSecs(10);
			if (time > _openTime) _openTime = time;
			else if (time.secsTo(_openTime) > 5) {
				LOG4CPLUS_ERROR(_logger, LOG4CPLUS_TEXT("something wrong on close return"));
			}
		}

	    OptionModel::OptionStatus OptionModel::status() const
	    {
			if (_openTime <= QDateTime::currentDateTime()) {
				return Idle;
			}
			return _status;
	    }

		bool OptionModel::isAvailable() const
		{
			return _openTime <= QDateTime::currentDateTime();
		}
    }
}
