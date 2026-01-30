#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString>
#include "AlbumDao.h"
#include "PictureDao.h"

class QSqlDatabase;

const QString DATABASE_FILENAME = "gallery.db";

class DatabaseManager
{
public:
    static DatabaseManager& instance(); //crea una sola instacia
    ~DatabaseManager();

protected:
    DatabaseManager (const QString& path = DATABASE_FILENAME);
    DatabaseManager& operator = (const DatabaseManager& rsh);

//puntero
private:
    QSqlDatabase* mDatabase;

public:
    const AlbumDao albumDao;
    const PictureDao pictureDao;
};

#endif // DATABASEMANAGER_H
