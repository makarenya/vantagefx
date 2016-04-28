//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTTYPE_H
#define VANTAGEFX_GWTTYPE_H

#include <vector>
#include <memory>
#include "GwtValue.h"
#include "GwtField.h"
#include "GwtObject.h"

class QDomElement;

namespace vantagefx {
    namespace api {

        class GwtObject;

        class GwtParser;

        class GwtField;

        class GwtType {
        public:

			explicit GwtType(std::string name)
				: _name(name) { }

            virtual ~GwtType() { }

            virtual void print(const GwtObject &object, std::ostream &stream, GwtPrintStyle style) const = 0;

            virtual void xml(const GwtObject &object, QDomElement &parent) const = 0;

            virtual void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const = 0;

			virtual std::string primary() const { return ""; }

			const std::string &name() const { return _name; }

			virtual std::shared_ptr<GwtIterator> iterateValues(const GwtConstObjectPtr &object) const;

        private:
            std::string _name;
        };

        class GwtComplexType : public GwtType {
        public:
            GwtComplexType(const std::string &name,
                           std::vector<std::shared_ptr<GwtField>> fields,
                           const std::string &primary);

            void print(const GwtObject &object, std::ostream &stream, GwtPrintStyle style) const override;

            void xml(const GwtObject &object, QDomElement &parent) const override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const override;

			GwtFieldPtr field(const std::string &name) const;

			std::string primary() const override;

			std::shared_ptr<GwtIterator> iterateValues(const GwtConstObjectPtr &object) const override;

		private:
            std::vector<std::shared_ptr<GwtField>> _fields;
            std::string _primary;
        };

        class GwtListType : public GwtType {
        public:
            explicit GwtListType(std::string name)
                    : GwtType(name) { }

            void print(const GwtObject &object, std::ostream &stream, GwtPrintStyle style) const override;

            void xml(const GwtObject &object, QDomElement &parent) const override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const override;

			std::shared_ptr<GwtIterator> iterateValues(const GwtConstObjectPtr &object) const override;
		};

        class GwtMapType : public GwtType {
        public:
            explicit GwtMapType(std::string name, int skip)
                    : GwtType(name), _skip(skip) { }

            void print(const GwtObject &object, std::ostream &stream, GwtPrintStyle style) const override;

            void xml(const GwtObject &object, QDomElement &parent) const override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const override;

		private:
            int _skip;
        };

		class GwtRequestType : public GwtType {
		public:
			GwtRequestType();

			void print(const GwtObject &object, std::ostream &stream, GwtPrintStyle style) const override;

			void xml(const GwtObject &object, QDomElement &parent) const override;

			void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const override;

		private:
			std::vector<std::shared_ptr<GwtField>> _fields;
		};

        typedef std::shared_ptr<GwtType> GwtTypePtr;
    }
}

#endif //VANTAGEFX_GWTTYPE_H
