//
// Created by Алексей Макареня on 01.05.16.
//

#include "ComboBoxModel.h"


namespace vantagefx {
    namespace viewmodel {

        ComboBoxModel::ComboBoxModel(QObject *parent) :
                QAbstractListModel(parent)
        {
        }

        const QStringList ComboBoxModel::comboList()
        {
            return _comboList;
        }

        void ComboBoxModel::setComboList(const QStringList &comboList)
        {
            beginResetModel();
            _comboList.clear();
            endResetModel();

            for(auto item : comboList) {
                beginInsertRows(QModelIndex(), _comboList.size(), _comboList.size());
                _comboList.push_back(item);
                endInsertRows();
            }
        }

        int ComboBoxModel::rowCount(const QModelIndex &parent) const {
            return _comboList.size();
        }

        QVariant ComboBoxModel::data(const QModelIndex &index, int role) const {
            if (!index.isValid()) return QVariant();
            if (index.row() >= _comboList.size()) return QVariant();
            if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
            return _comboList[index.row()];
        }

        QHash<int, QByteArray> ComboBoxModel::roleNames() const {
            QHash<int, QByteArray> roles;
            roles[Qt::DisplayRole] = "display";
            return roles;
        }
    }
}