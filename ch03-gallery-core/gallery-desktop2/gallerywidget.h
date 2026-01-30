#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include <QWidget>

class AlbumModel;
class ThumbnailProxyModel;
class QItemSelectionModel;
class PictureModel;

namespace Ui {
class GalleryWidget;
}

class GalleryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GalleryWidget(QWidget *parent = nullptr);
    ~GalleryWidget();

    void setAlbumModel(AlbumModel* model);
    void setAlbumSelectionModel(QItemSelectionModel* selectionModel);

    void setPictureModels(PictureModel* realModel, ThumbnailProxyModel* model);
    void setPictureSelectionModel(QItemSelectionModel* selectionModel);

signals:
    void pictureActivated(const QModelIndex& index);

private:
    Ui::GalleryWidget *ui;
};

#endif // GALLERYWIDGET_H
