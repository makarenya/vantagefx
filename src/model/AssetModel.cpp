//
// Created by AMakarenya on 11.05.2016.
//

#include "AssetModel.h"

namespace vantagefx {
    namespace model {

        AssetModel::AssetModel()
                : _id(0),
                  _price(0),
                  _marketId(0)
        {}

	    void AssetModel::setId(int id)
	    {
		    _id = id;
	    }

	    void AssetModel::setName(const QString &name)
	    {
		    _name = name;
	    }

	    void AssetModel::setPrice(double price)
	    {
		    _price = price;
	    }

	    void AssetModel::setMarketId(int marketId)
	    {
		    _marketId = marketId;
	    }

	    void AssetModel::setMarketName(const QString &marketName)
	    {
		    _marketName = marketName;
	    }

	    void AssetModel::setSubMarketName(const QString &subMarketName)
	    {
		    _subMarketName = subMarketName;
	    }

	    void AssetModel::setRate(const QString &name, int value)
	    {
		    _rates[name] = value;
	    }

	    int AssetModel::rate(const QString& name) const
	    {
		    if (!_rates.contains(name)) return 50;
		    return _rates[name];
	    }
    }
}
