#include "picturedao.h"
#include "picture.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

PictureDao::PictureDao(QSqlDatabase& database) :
    mDatabase(database)
{
}

void PictureDao::init() const
{
    if (!mDatabase.tables().contains("pictures")) {
        QSqlQuery query(mDatabase);
        query.exec(QString("CREATE TABLE pictures")
                   + " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   + "album_id INTEGER, "
                   + "url TEXT)");
    }
}

void PictureDao::addPictureInAlbum(int albumId, Picture& picture) const
{
    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO pictures (album_id, url) VALUES (:album_id, :url)");
    query.bindValue(":album_id", albumId);
    query.bindValue(":url", picture.fileUrl().toLocalFile());
    query.exec();

    picture.setId(query.lastInsertId().toInt());
}

void PictureDao::removePicture(int id) const
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM pictures WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
}

void PictureDao::removePicturesForAlbum(int albumId) const
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM pictures WHERE album_id = :album_id");
    query.bindValue(":album_id", albumId);
    query.exec();
}

QVector<Picture*> PictureDao::picturesForAlbum(int albumId) const
{
    QVector<Picture*> list;

    QSqlQuery query(mDatabase);
    query.prepare("SELECT * FROM pictures WHERE album_id = :album_id");
    query.bindValue(":album_id", albumId);
    query.exec();

    while (query.next()) {
        Picture* picture = new Picture(QUrl::fromLocalFile(query.value("url").toString()));
        picture->setId(query.value("id").toInt());
        picture->setAlbumId(query.value("album_id").toInt());
        list.append(picture);
    }

    return list;
}
