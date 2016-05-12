//
// Created by alexx on 27.04.2016.
//

#include "OptionModel.h"

namespace vantagefx {
    namespace model {

	    OptionModel::OptionModel()
				: _optionId(0),
	              _asset(nullptr),
	              _moneyBack(0),
	              _seconds(0),
			      _returnValue(0),
	              _price(0),
	              _close(0),
			      _checked(false),
			      _result(Processing), 
			      _bet(0)
			{}

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

        void OptionModel::setClose(int64_t close)
		{
            _close = close;
        }

		void OptionModel::updateDelay(const QDateTime &delay)
		{
			_delay = delay;
		}

		bool OptionModel::isDelayed() const
		{
			return QDateTime::currentDateTime() < _delay;
		}

		void OptionModel::closePosition(int64_t returned) 
		{
			if (returned == _bet) _result = Returned;
			else if (returned > _bet) _result = Successful;
			else _result = Failed;
		}

        void OptionModel::setChecked(bool checked)
		{
            _checked = checked;
        }

	    void OptionModel::bet(int64_t money)
	    {
			_bet = money;
			_result = Processing;
	    }

	    OptionModel::OptionStatus OptionModel::status() const
	    {
			if (isDelayed()) return _result;
			return _checked ? Selected : Idle;
	    }
    }
}