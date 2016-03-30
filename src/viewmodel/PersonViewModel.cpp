//
// Created by alexx on 13.03.2016.
//

#include "PersonViewModel.h"

PersonViewModel::PersonViewModel(PersonModel personModel) {
    _personModel = personModel;
}

void PersonViewModel::setName(const QString &name) {
    if (_personModel.name() == name) return;
    _personModel.setName(name);
    emit nameChanged(name);
}

void PersonViewModel::setSurname(const QString &surname) {
    if (_personModel.surname() == surname) return;
    _personModel.setSurname(surname);
    emit surnameChanged(surname);
}

void PersonViewModel::setPhone(const QString &phone) {
    if (_personModel.phone() == phone) return;
    _personModel.setPhone(phone);
    emit phoneChanged(phone);
}

void PersonViewModel::setAge(const int &age) {
    if (_personModel.age() == age) return;
    _personModel.setAge(age);
    emit ageChanged(age);
}

PersonListModel::~PersonListModel() {
    _list.clear();
}

QHash<int, QByteArray> PersonListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[NameRole] = "name";
    roles[SurnameRole] = "surname";
    roles[PhoneRole] = "phone";
    roles[AgeRole] = "age";
    return roles;
}

int PersonListModel::rowCount(const QModelIndex &parent) const {
    return _list.count();
}

QVariant PersonListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _list.size())
        return QVariant();

    auto person = _list[index.row()];
    switch (role) {
        case Qt::DisplayRole:
            return person.name() + " " + person.surname();
        case NameRole:
            return person.name();
        case SurnameRole:
            return person.surname();
        case PhoneRole:
            return person.phone();
        case AgeRole:
            return person.age();
        default:
            throw std::runtime_error("invalid role");
    }
}

void PersonListModel::push_back(const PersonModel &person) {
    beginInsertRows(QModelIndex(), _list.count(), _list.count());
    _list.push_back(person);
    endInsertRows();
}
