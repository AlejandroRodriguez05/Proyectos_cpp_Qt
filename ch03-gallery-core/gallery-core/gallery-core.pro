QT += sql
QT += core gui widgets

TEMPLATE = lib
DEFINES += GALLERYCORE_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Album.cpp \
    albumdao.cpp \
    albummodel.cpp \
    databasemanager.cpp \
    picture.cpp \
    picturedao.cpp \
    picturedelegate.cpp \
    picturemodel.cpp

HEADERS += \
    Album.h \
    albumdao.h \
    albummodel.h \
    databasemanager.h \
    gallery-core_global.h \
    picture.h \
    picturedao.h \
    picturedelegate.h \
    picturemodel.h

TRANSLATIONS += \
    gallery-core_es_ES.ts

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
