#include "picturemodel.h"
#include "DatabaseManager.h"
#include "AlbumModel.h"
#include <QFileInfo>

PictureModel::PictureModel(const AlbumModel& albumModel, QObject* parent) :
    QAbstractListModel(parent),
    mDb(DatabaseManager::instance()),
    mAlbumId(-1),
    mPictures()
{
    connect (&albumModel, &AlbumModel::rowsRemoved, this, &PictureModel::deletePicturesForAlbum);
}
void PictureModel::setAlbumId(int albumId)
{
    beginResetModel();
    mAlbumId = albumId;
    loadPictures(mAlbumId);
    endResetModel();
}
// === ADD PICTURE MODEL ===
QModelIndex PictureModel::addPicture(const Picture& picture)
{
    int row = mPictures.size();
    beginInsertRows(QModelIndex(), row, row);
    mPictures.append(new Picture(picture));
    endInsertRows();
    return index(row, 0);
}
// === ADD PICTURE BD AND MODEL ===
void PictureModel::addPicture(const QString& filePath, int albumId)
{
    Picture picture(filePath, albumId);
    mDb.pictureDao.addPictureInAlbum(albumId, picture);
    addPicture(picture);
}

// === LOAD PICTURES ===
void PictureModel::loadPictures(int albumId)
{
    beginResetModel();

    //si no hay album seleccionado vacia pictures
    if (albumId <= 0) {
        mPictures.clear();
        endResetModel();
        return;
    }
    //pide todas las pictures a la bd
    mPictures = mDb.pictureDao.picturesForAlbum(albumId);

    endResetModel();
}

// === ROW COUNT ===
int PictureModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mPictures.size();
}

// === DATA ===
QVariant PictureModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Picture* picture = mPictures.at(index.row());

    switch (role) {
    case IdRole:
        return picture->id();

    case FilePathRole:
        // Ruta de archivo del sistema
        return picture->fileUrl().toLocalFile();

    case Qt::DisplayRole:
        return QFileInfo(picture->fileUrl().toLocalFile()).fileName();


        case NameRole:
        return QFileInfo(picture->fileUrl().toLocalFile()).fileName();
    }

    return QVariant();
}

// === REMOVE ROWS ===
bool PictureModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if (row < 0 || row + count > mPictures.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        Picture* pic = mPictures.takeAt(row);
        delete pic;
    }

    endRemoveRows();
    return true;
}

// === REMOVE PICTURE ===
void PictureModel::removePicture(int row)
{
    if (row < 0 || row >= mPictures.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);

    // obtener la foto a eliminar
    Picture* picture = mPictures.at(row);

    // elimina de la base de datos
    mDb.pictureDao.removePicture(picture->id());

    // eliminar del vector del modelo
    mPictures.removeAt(row);

    // elimina el objeto
    delete picture;

    endRemoveRows();
}



// === DELETE PICTURES FOR ALBUM ===
void PictureModel::deletePicturesForAlbum()
{
    if (mAlbumId <= 0)
        return;

    mDb.pictureDao.removePicturesForAlbum(mAlbumId);
    loadPictures(mAlbumId);
}

// === ROL NAMES ===
QHash<int, QByteArray> PictureModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::FilePathRole] = "filePath";
    roles[Roles::NameRole] = "name";
    return roles;
}
