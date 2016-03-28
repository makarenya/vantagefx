//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTOBJECT_H
#define VANTAGEFX_GWTOBJECT_H


#include <memory>
#include <map>

namespace vantagefx {
    namespace api {

        class GwtType;
        class GwtParser;
        class GwtValue;


		enum class GwtPrintStyle
		{
			Text, // Просто текст значения
			Brief, // Краткий брифинг
			Xml // Полный xml
		};
		
		class GwtObject {
        public:
            explicit GwtObject(std::shared_ptr<GwtType> type);
            void print(std::ostream &stream, GwtPrintStyle style);
            std::map<std::string, std::shared_ptr<GwtValue>> values() const;
            std::shared_ptr<GwtValue> value(const std::string &name);
            void addValue(std::string name, std::shared_ptr<GwtValue> value);

        private:
            std::shared_ptr<GwtType> _type;
            std::map<std::string, std::shared_ptr<GwtValue>> _values;
        };

        typedef std::shared_ptr<GwtObject> GwtObjectPtr;
    }
}


#endif //VANTAGEFX_GWTOBJECT_H
