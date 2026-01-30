#ifndef ALBUM_H
#define ALBUM_H

#include "gallery-core_global.h"

//variable name
#include <QString>

//nombre de la clase
class GALLERY_CORE_EXPORT Album
{

//constructor
public:
    explicit Album(const QString& name = "");


//getters & setters
int id() const;
void setId(int id);

QString name() const;
void setName(const QString& name = ""); //(const para no modificarlo y & para no copiarlo sino referenciarlo)

//aqui se guardan datos reales para la BD
private:
int mId;
QString mName;

};
#endif // ALBUM_H
