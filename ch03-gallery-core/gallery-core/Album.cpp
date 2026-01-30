#include "Album.h"

Album::Album(const QString& name) :
    mId (-1), //se asigna -1 que despues la BD lo cambiará
    mName (name) //asigna el nombre de la variable name
{
}
int Album::id() const{
    return mId;
}
void Album::setId(int id) {
    mId = id;
}
QString Album::name() const{
    return mName;
}
void Album::setName(const QString& name) {
    mName = name;
}
