#ifndef PICTUREDAO_H
#define PICTUREDAO_H

#include <QVector>
#include "gallery-core_global.h"

class Picture;
class QSqlDatabase;

class GALLERY_CORE_EXPORT PictureDao
{
public:
    explicit PictureDao(QSqlDatabase& database);

    void init() const;
    void addPictureInAlbum(int albumId, Picture& picture) const;
    void removePicture(int id) const;
    void removePicturesForAlbum(int albumId) const;
    QVector<Picture*> picturesForAlbum(int albumId) const;

private:
    QSqlDatabase& mDatabase;
};

#endif // PICTUREDAO_H
