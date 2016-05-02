//
// Created by Алексей Макареня on 01.05.16.
//

#ifndef VANTAGEFX_COMBOBOXMODEL_H
#define VANTAGEFX_COMBOBOXMODEL_H

#include <QtCore>


namespace vantagefx {
    namespace viewmodel {

        class ComboBoxModel : public QAbstractListModel
        {
            Q_OBJECT

        public:
            ComboBoxModel(QObject *parent = 0);

            const QStringList comboList();
            void setComboList(const QStringList &comboList);

            Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

            Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

            QHash<int, QByteArray> roleNames() const override;

        private:

            QStringList _comboList;
        };
    }
}


#endif //VANTAGEFX_COMBOBOXMODEL_H
