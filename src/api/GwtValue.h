//
// Created by alexx on 26.03.2016.
//

#ifndef VANTAGEFX_GWTVALUE_H
#define VANTAGEFX_GWTVALUE_H


#include <iostream>
#include <memory>

namespace vantagefx {
    namespace api {

        enum class GwtValueType {
            Long, // Длинное целое, без вариантов
            Std, // Целое, которое может оказаться и строкой и булевым типом и целым
            Date, // Дата и только
            Integer, // Строго целое
            Float, // Число с плавающей точкой
            String, // Строка без вариантов
            Pointer // Указатель.
        };

        class GwtParser;

        class GwtValue;

        typedef std::shared_ptr<GwtValue> GwtValuePtr;

        class GwtValue {
        public:
            virtual void print(std::ostream &stream) = 0;

            virtual GwtValueType type() = 0;

            static GwtValuePtr parse(GwtParser &ctx, GwtValueType type);

            virtual int value() = 0;

        protected:
            virtual void parse(GwtParser &ctx) = 0;
        };
    }
}


#endif //VANTAGEFX_GWTVALUE_H
