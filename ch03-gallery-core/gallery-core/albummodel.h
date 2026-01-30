#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H

#include "gallery-core_global.h"
#include <QAbstractListModel>
#include <QVector>
#include <QItemSelectionModel>

class Album;

class GALLERY_CORE_EXPORT AlbumModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole
    };

    explicit AlbumModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    void addAlbum(const QString &name);
    void removeAlbum(int row);

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Album*> mAlbums;
};

#endif // ALBUMMODEL_H
