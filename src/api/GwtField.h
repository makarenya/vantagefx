//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTFIELD_H
#define VANTAGEFX_GWTFIELD_H

#include <string>
#include <ostream>
#include <memory>
#include <vector>
#include "GwtValue.h"

class QDomElement;

namespace vantagefx {
    namespace api {

        enum class GwtPrintStyle {
            MapKey, // Печатается для использования в качестве ключа 
			CsvValue, // Печатается для вставки в xml
			XmlFieldValue // Печатается для последующей вставки как значение в xml
        };

        class GwtParser;

		class GwtField;

		typedef std::shared_ptr<GwtField> GwtFieldPtr;

        class GwtField {
        public:
	        explicit GwtField(const std::string &name);

            virtual ~GwtField() { }

            std::string name() const;

            void setName(const std::string &name);

            virtual std::string type() const = 0;

            virtual GwtValue parse(GwtParser &parser) = 0;

            virtual void xml(const GwtValue &value, QDomElement &parent) const;

			virtual bool equal(const GwtValue &value, const GwtValue &other) const = 0;

            virtual void print(const GwtValue &value, std::ostream &stream, GwtPrintStyle style) const = 0;

            virtual std::string factor() const = 0;

        private:
            std::string _name;
        };

		GwtFieldPtr flng(const std::string &name);

		GwtFieldPtr fint(const std::string &name);

		GwtFieldPtr fstd(const std::string &name);

        GwtFieldPtr fdbl(const std::string &name);

        GwtFieldPtr fdte(const std::string &name);

        GwtFieldPtr fstr(const std::string &name);

        GwtFieldPtr fptr(const std::string &name);
    }
}


#endif //VANTAGEFX_GWTFIELD_H
