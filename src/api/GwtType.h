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

            virtual void printTable(std::ostream &stream) { }

            virtual void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) = 0;

            virtual void xml(GwtObject &object, QDomElement &parent) = 0;

            virtual void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) = 0;

            virtual bool empty() const { return true; }

			virtual std::string primary() const { return ""; }

            const std::string &name() const { return _name; }

			virtual void find(const GwtObject &object, const GwtValue &value, std::vector<std::string> &found, std::string prefix = "") const = 0;

			virtual std::shared_ptr<GwtValue> get(GwtObject &object, const std::string &path) = 0;

        private:
            std::string _name;
        };

        class GwtComplexType : public GwtType {
        public:
            GwtComplexType(const std::string &name,
                           const std::string &primary,
                           std::initializer_list<std::shared_ptr<GwtField>> fields);

            std::vector<std::shared_ptr<GwtField>> &fields();

            void printTable(std::ostream &stream) override;

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            void xml(GwtObject &object, QDomElement &parent) override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) override;

			std::shared_ptr<GwtField> field(std::string name);

            virtual bool empty() const override { return _created.size() == 0; }

			virtual std::string primary() const;

			virtual void find(const GwtObject &object, const GwtValue &value, std::vector<std::string> &found, std::string prefix = "") const override;
			
			virtual std::shared_ptr<GwtValue> get(GwtObject &object, const std::string &path) override;

		private:
            std::vector<std::weak_ptr<GwtObject>> _created;
            std::vector<std::shared_ptr<GwtField>> _fields;
            std::string _primary;
        };

        class GwtListType : public GwtType {
        public:
            explicit GwtListType(std::string name)
                    : GwtType(name) { }

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            void xml(GwtObject &object, QDomElement &parent) override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) override;
		
			virtual void find(const GwtObject &object, const GwtValue &value, std::vector<std::string> &found, std::string prefix = "") const override;

			virtual std::shared_ptr<GwtValue> get(GwtObject &object, const std::string &path) override;
		};

        class GwtMapType : public GwtType {
        public:
            explicit GwtMapType(std::string name, int skip)
                    : GwtType(name), _skip(skip) { }

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            void xml(GwtObject &object, QDomElement &parent) override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) override;

			virtual void find(const GwtObject &object, const GwtValue &value, std::vector<std::string> &found, std::string prefix = "") const override;

			virtual std::shared_ptr<GwtValue> get(GwtObject &object, const std::string &path) override;

		private:
            int _skip;
        };

		class GwtRequestType : public GwtType {
		public:
			GwtRequestType();

			void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

			void xml(GwtObject &object, QDomElement &parent) override;

			void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) override;

			virtual void find(const GwtObject &object, const GwtValue &value, std::vector<std::string> &found, std::string prefix = "") const override;

			virtual std::shared_ptr<GwtValue> get(GwtObject &object, const std::string &path) override;

		private:
			std::vector<std::shared_ptr<GwtField>> _fields;
		};

        class GwtSimpleType : public GwtType {
        public:
            GwtSimpleType(std::string name, std::shared_ptr<GwtField> type);

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            void xml(GwtObject &object, QDomElement &parent) override;

            void parse(GwtParser &parser, std::shared_ptr<GwtObject> &result) override;

			virtual void find(const GwtObject &object, const GwtValue &value, std::vector<std::string> &found, std::string prefix = "") const override;

			virtual std::shared_ptr<GwtValue> get(GwtObject &object, const std::string &path) override;

		private:
            std::shared_ptr<GwtField> _field;
        };

        typedef std::shared_ptr<GwtType> GwtTypePtr;
    }
}

#endif //VANTAGEFX_GWTTYPE_H
