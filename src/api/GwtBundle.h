//
// Created by alexx on 28.03.2016.
//

#ifndef VANTAGEFX_GWTBUNDLE_H
#define VANTAGEFX_GWTBUNDLE_H

#include <memory>
#include <string>
#include <map>

namespace vantagefx {
    namespace api {
        class GwtType;

        class GwtBundle {
        public:
            template<class T>
            void add(std::shared_ptr<T> type) {
                append(std::static_pointer_cast<GwtType>(type));
            }

            void append(std::shared_ptr<GwtType> type);
            std::shared_ptr<GwtType> type(std::string name);
            void printTables(std::string directory);
        private:
            std::map<std::string, std::shared_ptr<GwtType>> _types;
        };

        class GwtVantageFxBundle : public GwtBundle {
		public:
			GwtVantageFxBundle();
        };
    }
}


#endif //VANTAGEFX_GWTBUNDLE_H
