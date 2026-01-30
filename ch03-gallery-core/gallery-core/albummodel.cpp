#include "albummodel.h"
#include "albumdao.h"
#include "databasemanager.h"
#include "album.h"

AlbumModel::AlbumModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mAlbums = DatabaseManager::instance().albumDao.albums();
}

int AlbumModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAlbums.size();
}

void AlbumModel::addAlbum(const QString& name)
{
    beginInsertRows(QModelIndex(), mAlbums.size(), mAlbums.size());
    Album* album = new Album(name);
    DatabaseManager::instance().albumDao.addAlbum(*album);
    mAlbums.append(album);
    endInsertRows();
}


void AlbumModel::removeAlbum(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Album* album = mAlbums.takeAt(row);
    DatabaseManager::instance().albumDao.removeAlbum(album->id());
    delete album;
    endRemoveRows();
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Album* album = mAlbums.at(index.row());

    switch (role) {
    case IdRole:
        return album->id();
    case NameRole:
    case Qt::DisplayRole:
        return album->name();
    }

    return QVariant();
}

bool AlbumModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    Album* album = mAlbums.at(index.row());

    switch (role) {
    case NameRole:
    case Qt::EditRole:
        album->setName(value.toString());
        DatabaseManager::instance().albumDao.updateAlbum(*album);
        emit dataChanged(index, index, {role, Qt::DisplayRole});
        return true;
    default:
        return false;
    }
}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    return roles;
}
