#include "picturewidget.h"
#include "ui_picturewidget.h"

#include <QPushButton>
#include <QItemSelectionModel>

#include "ThumbnailProxyModel.h"
#include "PictureModel.h"

PictureWidget::PictureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureWidget),
    mRealModel(nullptr),
    mProxyModel(nullptr),
    mSelectionModel(nullptr)
{
    ui->setupUi(this);

    //eliminar picture
    connect(ui->deleteButton, &QPushButton::clicked,
            this, &PictureWidget::deletePicture);
    ui->pictureLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //volver a la galeria
    connect(ui->backButton, &QPushButton::clicked,
            this, &PictureWidget::backToGallery);

    //siguiente picture
    connect (ui->nextButton, &QPushButton::clicked,
            this, &PictureWidget::nextPicture);

    //anterior picture
    connect (ui->previousButton, &QPushButton::clicked,
            this, &PictureWidget::previousPicture);


}

// === SET MODELS ===
void PictureWidget::setModels(PictureModel* realModel, ThumbnailProxyModel* proxyModel)
{
    mRealModel = realModel;
    mProxyModel = proxyModel;
}

// === SET SELECTION MODEL
void PictureWidget::setSelectionModel(QItemSelectionModel* selectionModel)
{
    mSelectionModel = selectionModel;

    connect(mSelectionModel, &QItemSelectionModel::selectionChanged,
            this, [this](const QItemSelection &selected, const QItemSelection &) {
                loadPicture(selected);
            });
}

void PictureWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updatePicturePixmap();
}

// === PICTURE PIXMAP ===
void PictureWidget::updatePicturePixmap()
{
    if (mPixmap.isNull())
        return;

    ui->pictureLabel->setPixmap(
        mPixmap.scaled(ui->pictureLabel->size(), Qt::KeepAspectRatio)
        );

    ui->pictureLabel->setAlignment(Qt::AlignCenter);
}

// === LOAD PICTURE ===
void PictureWidget::loadPicture(const QItemSelection& selected)
{
    if (!mProxyModel || !mRealModel || selected.indexes().isEmpty())
        return;

    QModelIndex proxyIndex = selected.indexes().first();
    QModelIndex sourceIndex = mProxyModel->mapToSource(proxyIndex);

    QString fileName = mRealModel->data(sourceIndex, PictureModel::NameRole).toString();
    ui->nameLabel->setText(fileName);

    QString filePath = mRealModel->data(sourceIndex, PictureModel::FilePathRole).toString();
    mPixmap = QPixmap(filePath);
    updatePicturePixmap();
}

// === DELETE PICTURE ===
void PictureWidget::deletePicture()
{
    if (!mSelectionModel || !mProxyModel || !mRealModel)
        return;

    QModelIndex proxyIndex = mSelectionModel->currentIndex();
    if (!proxyIndex.isValid())
        return;

    QModelIndex sourceIndex = mProxyModel->mapToSource(proxyIndex);
    int row = sourceIndex.row();

    // Borrar del modelo real
    mRealModel->removePicture(row);

    // Si ya no quedan imagenes hace "volver a la galeria"
    if (mRealModel->rowCount() == 0) {
        emit backToGallery();
        return;
    }

    // Seleccionar la imagen anterior si existe
    int newRow = row - 1;
    if (newRow < 0)
        newRow = 0;

    //guarda la fila real y la pasa al proxy para ser compatible
    QModelIndex nuevoIndex = mRealModel->index(newRow, 0);
    QModelIndex nuevoProxyIndex = mProxyModel->mapFromSource(nuevoIndex);

    //cambiamos la seleccion y asi actualiza tambien
    mSelectionModel->setCurrentIndex(nuevoProxyIndex, QItemSelectionModel::SelectCurrent);
}

// === NEXT PICTURE ===
void PictureWidget::nextPicture()
{
    if (!mSelectionModel || !mProxyModel || !mRealModel)
        return;

    QModelIndex proxyIndex = mSelectionModel->currentIndex();
    if (!proxyIndex.isValid())
        return;

    QModelIndex sourceIndex = mProxyModel->mapToSource(proxyIndex);
    int row = sourceIndex.row();

    int total = mRealModel->rowCount();
    if (total == 0)
        return;

    int newRow = row + 1;
    if (newRow >= total)
        newRow = total - 1; // no pasar del final

    QModelIndex newSourceIndex = mRealModel->index(newRow, 0);
    QModelIndex newProxyIndex = mProxyModel->mapFromSource(newSourceIndex);

    mSelectionModel->setCurrentIndex(newProxyIndex, QItemSelectionModel::SelectCurrent);
}

// === PREVIOUS PICTURE ===
void PictureWidget::previousPicture()
{
    if (!mSelectionModel || !mProxyModel || !mRealModel)
        return;

    QModelIndex proxyIndex = mSelectionModel->currentIndex();
    if (!proxyIndex.isValid())
        return;

    QModelIndex sourceIndex = mProxyModel->mapToSource(proxyIndex);
    int row = sourceIndex.row();

    int newRow = row - 1;
    if (newRow < 0)
        newRow = 0; // no pasar del principio

    QModelIndex newSourceIndex = mRealModel->index(newRow, 0);
    QModelIndex newProxyIndex = mProxyModel->mapFromSource(newSourceIndex);

    mSelectionModel->setCurrentIndex(newProxyIndex, QItemSelectionModel::SelectCurrent);
}


PictureWidget::~PictureWidget()
{
    delete ui;
}
