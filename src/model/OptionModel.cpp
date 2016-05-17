//
// Created by alexx on 27.04.2016.
//

#include "OptionModel.h"
#include "TransactionModel.h"

namespace vantagefx {
    namespace model {

	    OptionModel::OptionModel()
				: _optionId(0),
	              _asset(nullptr),
	              _moneyBack(0),
	              _seconds(0),
			      _returnValue(0),
	              _price(0),
			      _checked(false), 
			      _status(Idle),
			      _openTime()
			{}

	    int OptionModel::lowRateValue() const
	    {
			return asset().rateValue("Call");
		}

	    int OptionModel::highRateValue() const
	    {
			return asset().rateValue("Put");
		}

	    void OptionModel::setOptionId(int64_t optionId)
		{
            _optionId = optionId;
        }

        void OptionModel::setAsset(AssetModel *asset)
		{
            _asset = asset;
        }

        void OptionModel::setMoneyBack(int moneyBack)
		{
            _moneyBack = moneyBack;
        }

        void OptionModel::setSeconds(int seconds)
		{
            _seconds = seconds;
        }

        void OptionModel::setReturnValue(int returnValue)
		{
            _returnValue = returnValue;
        }

        void OptionModel::setPrice(double price)
		{
            _price = price;
        }

        void OptionModel::setClose(QDateTime close)
		{
            _close = close;
        }

		void OptionModel::setChecked(bool checked)
		{
			_checked = checked;
		}

		void OptionModel::openTransaction()
	    {
			_status = Processing;
			_openTime = QDateTime::currentDateTime().addSecs(seconds() + 10);
	    }

	    void OptionModel::closeSuccess()
	    {
			_status = Successful;
			_openTime = QDateTime::currentDateTime().addSecs(10);
	    }

	    void OptionModel::closeFail()
	    {
			_status = Failed;
			_openTime = QDateTime::currentDateTime().addSecs(10);
		}

	    void OptionModel::closeReturn()
	    {
			_status = Returned;
			_openTime = QDateTime::currentDateTime().addSecs(10);
		}

	    OptionModel::OptionStatus OptionModel::status() const
	    {
			if (_openTime <= QDateTime::currentDateTime()) {
				return checked() ? Selected : Idle;
			}
			return _status;
	    }
    }
}