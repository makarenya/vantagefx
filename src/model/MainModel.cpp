//
// Created by alexx on 09.03.2016.
//

#include "MainModel.h"


void MainModel::setSourceValue(const QString &value) {
    _sourceValue = value;
}

void MainModel::setDestinationValue(const QString &value) {
    if (!value.isEmpty()) {
        _destinationValue = hashValue(value);
    }
    else {
        _destinationValue = value;
    }
}

QString MainModel::hashValue(const QString &value) {
    return QString(QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Md5).toHex());
}
