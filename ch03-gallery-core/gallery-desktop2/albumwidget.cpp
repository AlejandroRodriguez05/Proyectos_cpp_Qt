#include "AlbumWidget.h"
#include "ui_albumwidget.h"

#include <QInputDialog>
#include <QFileDialog>

#include "PictureDelegate.h"
#include "AlbumModel.h"
#include "PictureModel.h"
#include "ThumbnailProxyModel.h"

AlbumWidget::AlbumWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::AlbumWidget),
    mAlbumModel(nullptr),
    mAlbumSelectionModel(nullptr),
    mPictureModel(nullptr),
    mPictureSelectionModel(nullptr)
{
    ui->setupUi(this);
    clearUi();

    ui->thumbnailListView->setSpacing(5);
    ui->thumbnailListView->setResizeMode(QListView::Adjust);
    ui->thumbnailListView->setFlow(QListView::LeftToRight);
    ui->thumbnailListView->setWrapping(true);
    ui->thumbnailListView->setItemDelegate(new PictureDelegate(this));

    connect(ui->thumbnailListView, &QListView::doubleClicked,
            this, &AlbumWidget::pictureActivated);
    connect(ui->deleteButton, &QPushButton::clicked,
            this, &AlbumWidget::deleteAlbum);
    connect(ui->editButton, &QPushButton::clicked,
            this, &AlbumWidget::editAlbum);
    connect(ui->addPicturesButton, &QPushButton::clicked,
            this, &AlbumWidget::addPictures);
}

// === AÑADIR IMAGEN ===
void AlbumWidget::addPictures()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Add picture"));

    if (filename.isEmpty())
        return;

    QModelIndex albumIndex = mAlbumSelectionModel->currentIndex();
    if (!albumIndex.isValid())
        return;

    int albumId = mAlbumModel->data(albumIndex, AlbumModel::IdRole).toInt();

    // obtiene el metodo desde pictureModel
    mRealPictureModel->addPicture(filename, albumId);

}



// === EDITAR ALBUM
void AlbumWidget::editAlbum()
{
    QModelIndex currentAlbumIndex = mAlbumSelectionModel->currentIndex();
    if (!currentAlbumIndex.isValid())
        return;

    QString oldAlbumName = mAlbumModel->data(currentAlbumIndex, AlbumModel::NameRole).toString();

    bool ok;
    QString newName = QInputDialog::getText(this,
                                            "Album's name",
                                            "Change Album name",
                                            QLineEdit::Normal,
                                            oldAlbumName,
                                            &ok);

    if (ok && !newName.isEmpty()) {
        mAlbumModel->setData(currentAlbumIndex, newName, AlbumModel::NameRole);
    }
}



// === ELIMINAR ALBUM ===
void AlbumWidget::deleteAlbum()
{
    if (mAlbumSelectionModel->selectedIndexes().isEmpty()) {
        return;
    }

    int row = mAlbumSelectionModel->currentIndex().row();
    mAlbumModel->removeAlbum(row);

    // Seleccionar el album anterior
    QModelIndex previousModelIndex = mAlbumModel->index(row - 1, 0);
    if (previousModelIndex.isValid()) {
        mAlbumSelectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
        return;
    }

    // Seleccionar el siguiente album
    QModelIndex nextModelIndex = mAlbumModel->index(row, 0);
    if (nextModelIndex.isValid()) {
        mAlbumSelectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
        return;
    }

    // Si no queda ningun album, limpiar selección y UI
    mAlbumSelectionModel->clearSelection();
    clearUi();
}


// === CLEAR UI ===
void AlbumWidget::clearUi()
{
    ui->albumName->setText("");
    ui->deleteButton->setVisible(false);
    ui->editButton->setVisible(false);
    ui->addPicturesButton->setVisible(false);
}
void AlbumWidget::loadAlbum(const QModelIndex& albumIndex)
{
    auto pictureModel = qobject_cast<PictureModel*>(mPictureModel->sourceModel());
    pictureModel->setAlbumId(
        mAlbumModel->data(albumIndex, AlbumModel::IdRole).toInt()
        );

    ui->albumName->setText(
        mAlbumModel->data(albumIndex, Qt::DisplayRole).toString()
        );

    ui->deleteButton->setVisible(true);
    ui->editButton->setVisible(true);
    ui->addPicturesButton->setVisible(true);
}


// === setAlbumModel ===
void AlbumWidget::setAlbumModel(AlbumModel* albumModel)
{
    mAlbumModel = albumModel;
    connect(mAlbumModel, &QAbstractItemModel::dataChanged,
            this, [this](const QModelIndex &topLeft) {
                if (topLeft == mAlbumSelectionModel->currentIndex()) {
                    loadAlbum(topLeft);
                }
            });

}

// === AlbumSelectionModel ===
void AlbumWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel)
{
    mAlbumSelectionModel = albumSelectionModel;

    connect(mAlbumSelectionModel,
            &QItemSelectionModel::selectionChanged,
            [this] (const QItemSelection &selected) {
                if (selected.isEmpty()) {
                    clearUi();
                    return;
                }
                loadAlbum(selected.indexes().first());
            });

    // Si ya hay un album seleccionado al iniciar, lo carga
    QModelIndex current = mAlbumSelectionModel->currentIndex();
    if (current.isValid()) {
        loadAlbum(current);
    }
}


void AlbumWidget::setPictureModels(PictureModel* realModel, ThumbnailProxyModel* proxyModel)
{
    mRealPictureModel = realModel;
    mPictureModel = proxyModel;

    ui->thumbnailListView->setModel(mPictureModel);
}

void AlbumWidget::setPictureSelectionModel(QItemSelectionModel* selectionModel)
{
    ui->thumbnailListView->setSelectionModel(selectionModel);
}


AlbumWidget::~AlbumWidget()
{
    delete ui;
}
