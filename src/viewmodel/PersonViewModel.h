//
// Created by alexx on 13.03.2016.
//

#ifndef QT_SECOND_PERSONVIEWMODEL_H
#define QT_SECOND_PERSONVIEWMODEL_H

#include "src/model/PersonModel.h"

#include <QAbstractListModel>

class PersonViewModel : public QObject {
Q_OBJECT
    Q_PROPERTY(QString name
                       READ
                       name
                       WRITE
                       setName
                       NOTIFY
                       nameChanged)
    Q_PROPERTY(QString surname
                       READ
                       surname
                       WRITE
                       setSurname
                       NOTIFY
                       surnameChanged)
    Q_PROPERTY(QString phone
                       READ
                       phone
                       WRITE
                       setPhone
                       NOTIFY
                       phoneChanged)
    Q_PROPERTY(int age
                       READ
                       age
                       WRITE
                       setAge
                       NOTIFY
                       ageChanged)

private:
    PersonModel _personModel;

public:
    PersonViewModel(PersonModel personModel);

    virtual ~PersonViewModel() { }

    QString name() const { return _personModel.name(); }

    QString surname() const { return _personModel.surname(); }

    QString phone() const { return _personModel.phone(); }

    int age() const { return _personModel.age(); }

public slots:

    void setName(const QString &name);

    void setSurname(const QString &surname);

    void setPhone(const QString &phone);

    void setAge(const int &age);

signals:

    void nameChanged(const QString &name);

    void surnameChanged(const QString &surname);

    void phoneChanged(const QString &phone);

    void ageChanged(int age);
};

class PersonListModel : public QAbstractListModel {
Q_OBJECT

public:
    enum PersonRoles {
        NameRole = Qt::UserRole + 1,
        SurnameRole,
        PhoneRole,
        AgeRole
    };

    explicit PersonListModel(QObject *parent = 0)
            : QAbstractListModel(parent) { }

    virtual ~PersonListModel();

    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void push_back(const PersonModel &person);

private:
    QList<PersonModel> _list;
};

#endif //QT_SECOND_PERSONVIEWMODEL_H
