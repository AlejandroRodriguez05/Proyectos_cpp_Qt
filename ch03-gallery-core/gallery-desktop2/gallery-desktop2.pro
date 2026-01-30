QT += core gui widgets

TARGET = gallery-desktop2
TEMPLATE = app

SOURCES += \
    main.cpp \
    albumlistwidget.cpp \
    albumwidget.cpp \
    gallerywidget.cpp \
    mainwindow.cpp \
    picturewidget.cpp \
    thumbnailproxymodel.cpp

HEADERS += \
    albumlistwidget.h \
    albumwidget.h \
    gallerywidget.h \
    mainwindow.h \
    picturewidget.h \
    thumbnailproxymodel.h

FORMS += \
    albumlistwidget.ui \
    albumwidget.ui \
    gallerywidget.ui \
    mainwindow.ui \
    picturewidget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gallery-core/release/ -lgallery-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gallery-core/debug/ -lgallery-core
else:unix: LIBS += -L$$OUT_PWD/../gallery-core/ -lgallery-core

INCLUDEPATH += $$PWD/../gallery-core
DEPENDPATH += $$PWD/../gallery-core

RESOURCES += resource.qrc
