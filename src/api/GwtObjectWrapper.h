//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_GWTOBJECTWRAPPER_H
#define QT_SECOND_GWTOBJECTWRAPPER_H

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <string>
#include "CheckedType.h"
#include <cstddef>

namespace vantagefx {
    namespace api {

        struct IGwtObjectWrapper {
            virtual ~IGwtObjectWrapper() { }

            virtual std::string name() const = 0;
        };

        typedef std::shared_ptr <IGwtObjectWrapper> GwtObjectWrapperPtr;

        template<typename T>
        struct GwtObjectWrapper : IGwtObjectWrapper, boost::noncopyable {
	        explicit GwtObjectWrapper(const std::nullptr_t &) {
                _value = std::shared_ptr<std::nullptr_t>();
            }

            GwtObjectWrapper(T *v, std::string name) {
                _value = std::shared_ptr<T>(v);
                _name = name;
            }

            GwtObjectWrapper(const T &v, std::string name) {
                _value = std::make_shared<T>(v);
                _name = name;
            }

            GwtObjectWrapper(const std::shared_ptr<T> &v, std::string name) {
                _value = v;
                _name = name;
            }

            std::string name() const override { return _name; }
            std::shared_ptr<T> value() const { return _value; }

        private:
            std::shared_ptr<T> _value;
            std::string _name;
        };

        inline GwtObjectWrapperPtr wrap(const std::nullptr_t &) {
			using std::static_pointer_cast;
			using std::make_shared;
			using std::nullptr_t;
			return static_pointer_cast<IGwtObjectWrapper>(make_shared<GwtObjectWrapper<nullptr_t>>(nullptr));
        }

        template<typename T>
        GwtObjectWrapperPtr wrap(const T &value, std::string name) {
			using std::static_pointer_cast;
			using std::make_shared;
            return static_pointer_cast<IGwtObjectWrapper>(std::make_shared<GwtObjectWrapper<T>>(value, name));
        }

        template<typename T>
        GwtObjectWrapperPtr wrap(const std::shared_ptr<T> &v, std::string name) {
			using std::static_pointer_cast;
			using std::make_shared;
			return static_pointer_cast<IGwtObjectWrapper>(make_shared<GwtObjectWrapper<T>>(v, name));
        }

    }
}
#endif //QT_SECOND_GWTOBJECTWRAPPER_H
