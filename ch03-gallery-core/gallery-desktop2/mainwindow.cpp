#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStackedWidget>
#include <QItemSelectionModel>

#include "albummodel.h"
#include "picturemodel.h"
#include "thumbnailproxymodel.h"
#include "gallerywidget.h"
#include "picturewidget.h"

// === CONSTRUCTOR ===
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    mStackedWidget(new QStackedWidget(this)),
    mGalleryWidget(new GalleryWidget(this)),
    mPictureWidget(new PictureWidget(this))
{
    ui->setupUi(this);

    AlbumModel* albumModel = new AlbumModel(this);
    QItemSelectionModel* albumSelectionModel = new QItemSelectionModel(albumModel); //saber que album esta selccionado
    albumSelectionModel->setCurrentIndex(albumModel->index(0, 0), QItemSelectionModel::SelectCurrent); //crea un indice en 0, 0

    PictureModel* pictureModel = new PictureModel(*albumModel, this); //modelo picture con todas las imagenes
    ThumbnailProxyModel* thumbnailModel = new ThumbnailProxyModel(this); //crea el proxy
    thumbnailModel->setSourceModel(pictureModel); // proxy saca datos de pictureModel

    QItemSelectionModel* pictureSelectionModel = new QItemSelectionModel(thumbnailModel); //selection model con el proxy para coincidir con la vista

    this->setWindowTitle("Gallery");
    this->setWindowIcon(QIcon(":/icons/galeria.jpg"));


    //pasa los objetos a GalleryWidget
    mGalleryWidget->setAlbumModel(albumModel);
    mGalleryWidget->setPictureModels(pictureModel, thumbnailModel);

    mGalleryWidget->setAlbumSelectionModel(albumSelectionModel);
    mGalleryWidget->setPictureSelectionModel(pictureSelectionModel);

    albumSelectionModel->setCurrentIndex(albumModel->index(0, 0), QItemSelectionModel::SelectCurrent); // vuelve al inicio para que todos los widget

    //pasa los objetos a PictureWidget
    mPictureWidget->setModels(pictureModel, thumbnailModel);
    mPictureWidget->setSelectionModel(pictureSelectionModel);

    //si hace dobleclick muestra la picture
    connect(mGalleryWidget, &GalleryWidget::pictureActivated,this, &MainWindow::displayPicture);

    //si le da Back vuelve a GalleryWidget
    connect(mPictureWidget, &PictureWidget::backToGallery,this, &MainWindow::displayGallery);

 // === DISPLAY ===
    //añade los widget para despues mostrarlo
    mStackedWidget->addWidget(mGalleryWidget);
    mStackedWidget->addWidget(mPictureWidget);
    displayGallery();
    setCentralWidget(mStackedWidget);
}

void MainWindow::displayGallery()
{
    mStackedWidget->setCurrentWidget(mGalleryWidget);
}

void MainWindow::displayPicture(const QModelIndex& /*index*/)
{
    mStackedWidget->setCurrentWidget(mPictureWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
