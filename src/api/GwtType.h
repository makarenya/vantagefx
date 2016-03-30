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


namespace vantagefx {
    namespace api {
        class GwtObject;

        class GwtParser;

        class GwtField;

        class GwtType : public std::enable_shared_from_this<GwtType> {
        public:

            explicit GwtType(std::string name) : _name(name) { }

            virtual ~GwtType() { }

            virtual void printTable(std::ostream &stream) { }

            virtual void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) = 0;

            virtual std::shared_ptr<GwtObject> parse(GwtParser &parser) = 0;

            virtual bool empty() const { return true; }

            const std::string &name() const { return _name; }

        private:
            std::string _name;
        };

        class GwtComplexType : public GwtType {
        public:
            GwtComplexType(std::string name, std::vector<GwtField> fields, int primary = -1);

            std::vector<GwtField> fields() const;

            virtual void printTable(std::ostream &stream) override;

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            std::shared_ptr<GwtObject> parse(GwtParser &parser) override;

            virtual bool empty() const { return _created.size() == 0; }

        private:
            std::vector<std::weak_ptr<GwtObject>> _created;
            std::vector<GwtField> _fields;
            int _primary;
        };

        class GwtListType : public GwtType {
        public:
            explicit GwtListType(std::string name) : GwtType(name) { }

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            std::shared_ptr<GwtObject> parse(GwtParser &parser) override;
        };

        class GwtMapType : public GwtType {
        public:
            explicit GwtMapType(std::string name, int skip) : GwtType(name), _skip(skip) { }

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            std::shared_ptr<GwtObject> parse(GwtParser &parser) override;

        private:
            int _skip;
        };

        class GwtSimpleType : public GwtType {
        public:
            GwtSimpleType(std::string name, GwtValueType type);

            void print(GwtObject &object, std::ostream &stream, GwtPrintStyle style) override;

            std::shared_ptr<GwtObject> parse(GwtParser &parser) override;

        private:
            GwtValueType _type;
        };

        typedef std::shared_ptr<GwtType> GwtTypePtr;
    }
}

#endif //VANTAGEFX_GWTTYPE_H
