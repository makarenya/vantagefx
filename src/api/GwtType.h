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
				: _pointer(fptr("pointer")), 
				_name(name) { }

            virtual ~GwtType() { }

            virtual void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) = 0;

            virtual void xml(GwtObject &object, QDomElement &parent) = 0;

            virtual void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const = 0;

			virtual std::string primary() const { return ""; }

            const std::string &name() const { return _name; }

			virtual std::shared_ptr<GwtIterator> iterateValues(GwtObjectPtr &object, 
				GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path);

		protected:
			GwtFieldPtr _pointer;

        private:
            std::string _name;
        };

        class GwtComplexType : public GwtType {
        public:
            GwtComplexType(const std::string &name,
                           const std::string &primary,
                           std::initializer_list<std::shared_ptr<GwtField>> fields);

            std::vector<std::shared_ptr<GwtField>> &fields();

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            void xml(GwtObject &object, QDomElement &parent) override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const override;

			std::shared_ptr<GwtField> field(std::string name);

			std::string primary() const override;

			virtual std::shared_ptr<GwtIterator> iterateValues(GwtObjectPtr &object,
				GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path);

		private:
            std::vector<std::shared_ptr<GwtField>> _fields;
            std::string _primary;
        };

        class GwtListType : public GwtType {
        public:
            explicit GwtListType(std::string name)
                    : GwtType(name) { }

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            void xml(GwtObject &object, QDomElement &parent) override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const override;

			std::shared_ptr<GwtIterator> iterateValues(GwtObjectPtr &object, GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path) override;
		};

        class GwtMapType : public GwtType {
        public:
            explicit GwtMapType(std::string name, int skip)
                    : GwtType(name), _skip(skip) { }

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            void xml(GwtObject &object, QDomElement &parent) override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const override;

		private:
            int _skip;
        };

		class GwtRequestType : public GwtType {
		public:
			GwtRequestType();

			void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

			void xml(GwtObject &object, QDomElement &parent) override;

			void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) const override;

		private:
			std::vector<std::shared_ptr<GwtField>> _fields;
		};

        typedef std::shared_ptr<GwtType> GwtTypePtr;
    }
}

#endif //VANTAGEFX_GWTTYPE_H
