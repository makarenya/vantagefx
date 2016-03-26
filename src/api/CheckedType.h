//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_CHECKEDTYPE_H
#define QT_SECOND_CHECKEDTYPE_H

namespace vantagefx {
	namespace api {

		template<typename T>
		struct CheckedType {
			explicit CheckedType(T& value) : _value(value) { }
            T &value() const { return _value; }
        private:
			T& _value;
		};

        template<typename T>
        CheckedType<T> checked(T &value) { return CheckedType<T>(value); }
    }
}


#endif //QT_SECOND_CHECKEDTYPE_H
