//
// Created by alexx on 09.03.2016.
//

#ifndef QT_SECOND_MAINMODEL_H
#define QT_SECOND_MAINMODEL_H

#include <QtCore>

class MainModel
{
private:
    QString _sourceValue;
    QString _destinationValue;

public:
    virtual ~MainModel() {}

    QString sourceValue() const { return _sourceValue; }

    void setSourceValue(const QString &value);

    QString destinationValue() const { return _destinationValue; }

    void setDestinationValue(const QString &value);

protected:
    QString hashValue(const QString &value);
};


#endif //QT_SECOND_MAINMODEL_H
