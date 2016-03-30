//
// Created by alexx on 13.03.2016.
//

#include "PersonModel.h"

PersonModel::PersonModel()
        : _age(0) { }

PersonModel &PersonModel::setName(const QString &name) {
    _name = name;
    return *this;
}

PersonModel &PersonModel::setSurname(const QString &surname) {
    _surname = surname;
    return *this;
}

PersonModel &PersonModel::setPhone(const QString &phone) {
    _phone = phone;
    return *this;
}

PersonModel &PersonModel::setAge(int age) {
    _age = age;
    return *this;
}
