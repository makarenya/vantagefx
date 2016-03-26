//
// Created by alexx on 09.03.2016.
//
#include "MainViewModel.h"

MainViewModel::MainViewModel(const MainModel &mainModel)
{
    _mainModel = mainModel;
}

MainViewModel::~MainViewModel()
{
}

void MainViewModel::clearCommand()
{
    QString emptyString = "";
    _phonebook.push_back(PersonModel()
         .setName("test")
         .setSurname("tester")
         .setPhone("6666666")
         .setAge(0));
    setSourceValue(emptyString);
}

void MainViewModel::setSourceValue(const QString &value)
{
    if (_mainModel.sourceValue() == value) return;
    _mainModel.setSourceValue(value);
    emit sourceChanged(value);
    setDestinationValue(value);
}

void MainViewModel::setDestinationValue(const QString &value)
{
    if (_mainModel.destinationValue() == value) return;
    _mainModel.setDestinationValue(value);
    emit destinationChanged(value);
}

void MainViewModel::addPerson(const PersonModel &person)
{
    _phonebook.push_back(person);
    emit phonebookChanged(&_phonebook);
}
