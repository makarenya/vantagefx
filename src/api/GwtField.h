//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTFIELD_H
#define VANTAGEFX_GWTFIELD_H


#include "GwtValue.h"

namespace vantagefx {
    namespace api {

        class GwtField {
        public:
            GwtField(GwtValueType type, std::string name);

            GwtValueType type() const;
            std::string name() const;
            void setName(std::string name);

        private:
            GwtValueType _type;
            std::string _name;
        };

        inline void GwtField::setName(std::string name) {
            _name = name;
        }

        inline GwtValueType GwtField::type() const {
            return _type;
        }

        inline std::string GwtField::name() const {
            return _name;
        }

        inline GwtField LongField(std::string name)
        {
            return GwtField(GwtValueType::Long, name);
        }
        inline GwtField LongField()
        {
            return GwtField(GwtValueType::Long, "");
        }
        inline GwtField StdField(std::string name)
        {
            return GwtField(GwtValueType::Std, name);
        }
        inline GwtField StdField()
        {
            return GwtField(GwtValueType::Std, "");
        }
        inline GwtField IntField(std::string name)
        {
            return GwtField(GwtValueType::Integer, name);
        }
        inline GwtField IntField()
        {
            return GwtField(GwtValueType::Integer, "");
        }
        inline GwtField DateField(std::string name)
        {
            return GwtField(GwtValueType::Date, name);
        }
        inline GwtField FloatField(std::string name)
        {
            return GwtField(GwtValueType::Float, name);
        }
        inline GwtField FloatField()
        {
            return GwtField(GwtValueType::Float, "");
        }
        inline GwtField StringField(std::string name)
        {
            return GwtField(GwtValueType::String, name);
        }
        inline GwtField StringField()
        {
            return GwtField(GwtValueType::String, "");
        }
        inline GwtField PtrField(std::string name)
        {
            return GwtField(GwtValueType::Pointer, name);
        }
        inline GwtField PtrField()
        {
            return GwtField(GwtValueType::Pointer, "");
        }
    }
}


#endif //VANTAGEFX_GWTFIELD_H
