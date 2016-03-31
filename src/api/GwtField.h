//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTFIELD_H
#define VANTAGEFX_GWTFIELD_H

#include <string>
#include <ostream>
#include <memory>
#include <vector>

class QDomElement;

namespace vantagefx {
    namespace api {

        enum class GwtPrintStyle {
            MapKey, // Печатается для использования в качестве ключа 
			CsvValue, // Печатается для вставки в xml
			XmlFieldValue // Печатается для последующей вставки как значение в xml
        };

        class GwtValue;

        class GwtParser;

        class GwtField {
        public:
	        explicit GwtField(const std::string &name);

            virtual ~GwtField() { }

            std::string name() const;

            void setName(const std::string &name);

            virtual std::string type() const = 0;

            virtual std::shared_ptr<GwtValue> parse(GwtParser &parser) = 0;

            virtual void xml(std::shared_ptr<GwtValue> &value, QDomElement &parent) const;

			virtual void find(std::shared_ptr<GwtValue> &value, const GwtValue &search, std::vector<std::string> &found, std::string prefix = "") const;

			virtual bool equal(const GwtValue &value, const GwtValue &other) const = 0;

			virtual std::shared_ptr<GwtValue> get(std::shared_ptr<GwtValue> &value, const std::string &path);

            virtual void print(std::shared_ptr<GwtValue> &value, std::ostream &stream, GwtPrintStyle style) const = 0;

            virtual std::string factor() const = 0;

        private:
            std::string _name;
        };

        std::shared_ptr<GwtField> flng(const std::string &name);

        std::shared_ptr<GwtField> fint(const std::string &name);

        std::shared_ptr<GwtField> fstd(const std::string &name);

        std::shared_ptr<GwtField> fdbl(const std::string &name);

        std::shared_ptr<GwtField> fdte(const std::string &name);

        std::shared_ptr<GwtField> fstr(const std::string &name);

        std::shared_ptr<GwtField> fptr(const std::string &name);
    }
}


#endif //VANTAGEFX_GWTFIELD_H
