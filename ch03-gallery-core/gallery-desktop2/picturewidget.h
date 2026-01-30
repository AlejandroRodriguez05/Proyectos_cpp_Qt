#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>
#include <QItemSelection>
#include <QPixmap>

class ThumbnailProxyModel;
class PictureModel;
class QItemSelectionModel;

namespace Ui {
class PictureWidget;
}

class PictureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PictureWidget(QWidget *parent = 0);
    ~PictureWidget();

    void setModels(PictureModel* realModel, ThumbnailProxyModel* proxyModel);
    void setSelectionModel(QItemSelectionModel* selectionModel);

    void previousPicture();
    void nextPicture();

signals:
    void backToGallery();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void loadPicture(const QItemSelection& selected);
    void deletePicture();

private:
    void updatePicturePixmap();

private:
    Ui::PictureWidget* ui;
    PictureModel* mRealModel;
    ThumbnailProxyModel* mProxyModel;
    QItemSelectionModel* mSelectionModel;
    QPixmap mPixmap;
};

#endif // PICTUREWIDGET_H
