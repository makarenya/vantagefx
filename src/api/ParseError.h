//
// Created by alexx on 25.03.2016.
//

#ifndef QT_SECOND_PARSEERROR_H
#define QT_SECOND_PARSEERROR_H

#include <stdexcept>

struct ParseError : std::runtime_error {
    explicit ParseError(std::string which) : std::runtime_error(which) { }
};

struct UnexpectedType : ParseError {
    explicit UnexpectedType(std::string found, std::string expected)
            : ParseError("unexpected type " + found + ", expecting: " + expected) { }
};


#endif //QT_SECOND_PARSEERROR_H
