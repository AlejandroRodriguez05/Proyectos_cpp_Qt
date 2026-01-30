#include "albumlistwidget.h"
#include "ui_albumlistwidget.h"

#include <QInputDialog>

#include "AlbumModel.h"

AlbumListWidget::AlbumListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlbumListWidget),
    mAlbumModel(nullptr)
{
    ui->setupUi(this);

    connect(ui->createAlbumButton, &QPushButton::clicked, this, &AlbumListWidget::createAlbum);
}

AlbumListWidget::~AlbumListWidget()
{
    delete ui;
}

void AlbumListWidget::setModel(AlbumModel* model)
{
    mAlbumModel = model;
    ui->albumList->setModel(mAlbumModel);
}

void AlbumListWidget::setSelectionModel(QItemSelectionModel* selectionModel)
{
    ui->albumList->setSelectionModel(selectionModel);
}

void AlbumListWidget::createAlbum()
{
    if (!mAlbumModel)
        return;

    bool ok = false;
    QString albumName = QInputDialog::getText(
        this,
        tr("Nuevo album"),
        tr("Nombre del album:"),
        QLineEdit::Normal,
        "",
        &ok);

    if (!ok || albumName.trimmed().isEmpty())
        return;

    // Crear el album (no devuelve indice)
    mAlbumModel->addAlbum(albumName);

    // Seleccionar la ultima fila (el album recien añadido)
    int lastRow = mAlbumModel->rowCount() - 1;

    if (lastRow >= 0 && ui->albumList->selectionModel()) {
        QModelIndex index = mAlbumModel->index(lastRow, 0);
        ui->albumList->selectionModel()->setCurrentIndex(
            index,
            QItemSelectionModel::ClearAndSelect
            );
    }
}

