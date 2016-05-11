//
// Created by AMakarenya on 10.05.2016.
//

#ifndef VANTAGEFX_VANTAGEFXMODEL_H
#define VANTAGEFX_VANTAGEFXMODEL_H

#include <QtCore>
#include "../api/GwtObject.h"
#include "GwtOptionModel.h"

namespace vantagefx {
    namespace model {

        class VantageFxModel {
        public:
            void setLut(api::GwtObjectPtr lut);
            int instrumentTypeId() const;
            QStringList servers() const;
            int rateId(QString name);
            std::vector<GwtOptionModel> options() const;
	        GwtOptionModel optionInfo(int64_t optionId) const;
			
			void setAccount(api::GwtObjectPtr auth);
			int64_t accountId() const;
			bool isLoggedIn() const;
			QString userName() const;

			void updateOptions(api::GwtObjectPtr refresh);
			int assetId(int64_t optionId) const;
	        int64_t currentPrice(int assetId) const;
	        int64_t currentMoney() const;

        private:
			void loadOption(const api::GwtValue &value, GwtOptionModel &model) const;
			
			api::GwtObjectPtr _lut;
			api::GwtObjectPtr _auth;
			api::GwtObjectPtr _refresh;
            QMap<QString, int> _rates;
            api::GwtValue _openId;
        };
    }
}


#endif //VANTAGEFX_VANTAGEFXMODEL_H
