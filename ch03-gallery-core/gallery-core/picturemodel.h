#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include <QAbstractListModel>
#include "gallery-core_global.h"
#include "Picture.h"

class Album;
class DatabaseManager;
class AlbumModel;

class GALLERY_CORE_EXPORT PictureModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        FilePathRole,
        NameRole
    };

    PictureModel(const AlbumModel& albumModel, QObject* parent = 0);

    //añade una foto al modelo
    QModelIndex addPicture(const Picture& picture);
    //guarda en BD y añade al modelo
    void addPicture(const QString& filePath, int albumId);


    //devuelve cuantas filas existen (fotos)
    int rowCount (const QModelIndex& parent = QModelIndex()) const override;

    //devuelve los datos de una foto dependiendo del rol
    QVariant data(const QModelIndex& index, int role) const override;

    //elimina el n1 de filas elegidas empezando desde (row)
    bool removeRows(int row, int count, const QModelIndex& parent) override;

    //pone el id del album para mostrar sus fotos
    void setAlbumId(int albumId);

    //limpia todas las fotos cargadas en el modelo
    void clearAlbum();

public slots:
    //elimina todas las fotos del album
    void deletePicturesForAlbum();
    //elimina una foto del album
    void removePicture(int row);

protected:
    QHash<int, QByteArray> roleNames() const override;


private:
    //carga las fotos del album
    void loadPictures(int albumId);

    //comprueba si el index es valido
    bool isIndexValid(const QModelIndex& index) const;

private:
    DatabaseManager& mDb;
    int mAlbumId;
    QVector<Picture*> mPictures;

};

#endif // PICTUREMODEL_H
