//
// Created by alexx on 13.03.2016.
//

#ifndef QT_SECOND_PERSONMODEL_H
#define QT_SECOND_PERSONMODEL_H

#include <QtCore>

class PersonModel {
private:
    QString _name;
    QString _surname;
    QString _phone;
    int _age;

public:
    PersonModel();

    virtual ~PersonModel() { }

    QString name() const { return _name; }

    PersonModel &setName(const QString &name);

    QString surname() const { return _surname; }

    PersonModel &setSurname(const QString &surname);

    QString phone() const { return _phone; }

    PersonModel &setPhone(const QString &phone);

    int age() const { return _age; }

    PersonModel &setAge(int age);
};


#endif //QT_SECOND_PERSONMODEL_H
