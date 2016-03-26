//
// Created by alexx on 09.03.2016.
//

#ifndef QT_SECOND_MAINVIEWMODEL_H
#define QT_SECOND_MAINVIEWMODEL_H

#include <QtCore>
#include "src/model/MainModel.h"
#include "PersonViewModel.h"

class MainViewModel : public QObject
{
    Q_OBJECT
public:
    MainModel _mainModel;
    PersonListModel _phonebook;

    Q_PROPERTY(QString sourceValue READ sourceValue WRITE setSourceValue NOTIFY sourceChanged);
    Q_PROPERTY(QString destinationValue READ destinationValue WRITE setDestinationValue NOTIFY destinationChanged)
    Q_PROPERTY(PersonListModel* phonebook READ phonebook NOTIFY phonebookChanged)

public:
    explicit MainViewModel(const MainModel &mainModel);
    virtual ~MainViewModel();
    Q_INVOKABLE void clearCommand();
    QString sourceValue() const { return _mainModel.sourceValue(); }
    QString destinationValue() const { return _mainModel.destinationValue(); }
    PersonListModel *phonebook() { return &_phonebook; }

public slots:
    void setSourceValue(const QString &value);
    void setDestinationValue(const QString &value);
    void addPerson(const PersonModel &person);

signals:
    void sourceChanged(const QString &arg);
    void destinationChanged(const QString &arg);
    void phonebookChanged(const PersonListModel *phonebook);
};


#endif //QT_SECOND_MAINVIEWMODEL_H
