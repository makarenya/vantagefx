//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_PARSEERROR_H
#define QT_SECOND_PARSEERROR_H

#include <stdexcept>
#include <sstream>

struct ParseError : std::runtime_error {
    explicit ParseError(std::string which)
            : std::runtime_error(which) { }
};

struct UnexpectedType : ParseError {
    explicit UnexpectedType(std::string found, std::string expected)
            : ParseError(BuildMessage(found, expected)) { }

    static std::string BuildMessage(std::string found, std::string expected) {
        std::stringstream message;
        message << "unexpected type " << found << ", expecting: " << expected;
        return message.str();
    }
};

struct WrongGwtType : ParseError {
    WrongGwtType(std::string name, std::string fieldName, std::string expected, std::string found)
            : ParseError(BuildMessage(name, fieldName, expected, found)) { }

    static std::string BuildMessage(std::string name, std::string fieldName, std::string expected, std::string found) {
        std::stringstream message;
        message << name << "::" << fieldName << " parsed as "
        << expected << " but actual type is "
        << found << std::endl;
        return message.str();
    }
};


#endif //QT_SECOND_PARSEERROR_H
