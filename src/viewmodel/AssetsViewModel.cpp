//
// Created by AMakarenya on 01.06.2016.
//

#include "AssetsViewModel.h"

namespace vantagefx {
	namespace viewmodel {

		QHash<int, QByteArray> AssetsViewModel::roleNames() const
		{
			QHash<int, QByteArray> roles;
            roles[Qt::DisplayRole] = "display";
			return roles;
		}

		void AssetsViewModel::setList(const QMap<int, model::AssetModel> &list)
		{
			beginResetModel();
			_assets.clear();
			endResetModel();

			QMap<QString, QPair<int, QString>> tmp;
			for (auto &asset : list) {
				auto key = QString("%1_%2").arg(asset.marketId(), 6, QChar('0')).arg(asset.id(), 6, QChar('0'));
				tmp[key] = QPair<int, QString>(asset.id(), asset.name());
			}

			for (auto it = tmp.cbegin(); it != tmp.cend(); ++it) {
				beginInsertRows(QModelIndex(), _assets.size(), _assets.size());
				_assets.push_back(*it);
				endInsertRows();
			}
		}

		int AssetsViewModel::rowCount(const QModelIndex &parent) const
		{
			return _assets.size();
		}

		QVariant AssetsViewModel::data(const QModelIndex &index, int role) const
		{
			if (!index.isValid()) return QVariant();
			if (index.row() >= _assets.size()) return QVariant();
			switch (role) {
			case IdRole:
				return _assets[index.row()].first;
			case Qt::DisplayRole:
			case NameRole:
			case Qt::EditRole:
				return _assets[index.row()].second;
			default:
				return QVariant();
			}
		}

		int AssetsViewModel::assetId(int index) const
		{
			if (index < 0 || index >= _assets.size()) {
				throw std::out_of_range("index out of range");
			}
			return _assets[index].first;
		}
	}
}