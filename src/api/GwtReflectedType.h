//
// Created by AMakarenya on 23.03.2016.
//

#ifndef QT_SECOND_GWTREFLECTEDTYPE_H
#define QT_SECOND_GWTREFLECTEDTYPE_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstddef>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ParseError.h"

namespace vantagefx {
    namespace api {

        template<typename T>
        struct GwtReflectedType;

        template<typename T>
        struct GwtReflectedType<std::vector<T>> {
            static std::string className() { return "java.util.ArrayList"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

		template<typename T>
		struct GwtReflectedType<std::set<T>> {
			static std::string className() { return "java.util.HashSet"; }
			static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
		};

		template<typename Key, typename Value>
        struct GwtReflectedType<std::map<Key, Value>> {
            static std::string className() { return "java.util.HashMap"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        template<>
        struct GwtReflectedType<int> {
            static std::string className() { return "java.lang.Integer"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

        template<>
        struct GwtReflectedType<std::string> {
            static std::string className() { return "java.lang.String"; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };

		template<>
		struct GwtReflectedType<boost::posix_time::ptime> {
			static std::string className() { return "java.util.Date"; }
			static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
		};
		

        template<>
        struct GwtReflectedType<std::nullptr_t> {
            static std::string className() { return ""; }
            static void check(std::string name) { if (name != className()) throw UnexpectedType(name, className()); }
        };
    }
}

#endif //QT_SECOND_GWTREFLECTEDTYPE_H
