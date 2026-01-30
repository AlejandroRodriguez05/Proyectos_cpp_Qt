#include "gallerywidget.h"
#include "ui_gallerywidget.h"

// === CONSTRUCTOR ===
GalleryWidget::GalleryWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GalleryWidget) //crea la ui con todos los widget
{
    ui->setupUi(this); //carga la interfaz
}

//pasa el modelo de Album a los widget para ponerlo en la interfaz
void GalleryWidget::setAlbumModel(AlbumModel* model)
{
    ui->albumListWidget->setModel(model);
    ui->albumWidget->setAlbumModel(model);
}

//cuando un usuario selecciona un album se lo pasa a los widget
void GalleryWidget::setAlbumSelectionModel(QItemSelectionModel* selectionModel)
{
    ui->albumListWidget->setSelectionModel(selectionModel); // selectionModel para saber que album se seleccionó
    ui->albumWidget->setAlbumSelectionModel(selectionModel); //carga las imagenes del album seleccionado
}

//metodo para mostrar las fotos del album
void GalleryWidget::setPictureModels(PictureModel* realModel,ThumbnailProxyModel* proxyModel)
{
    ui->albumWidget->setPictureModels(realModel, proxyModel);//"real" para todas las fotos - "proxy" modelo resumido para las miniaturas
}


void GalleryWidget::setPictureSelectionModel(QItemSelectionModel* selectionModel)
{
    //para saber que foto esta seleccionada
    ui->albumWidget->setPictureSelectionModel(selectionModel);

    //cuando el usuario entre en la foto AlbumWidget y Gallery harán pictureActived
    connect(ui->albumWidget, &AlbumWidget::pictureActivated,
            this, &GalleryWidget::pictureActivated);
}

// === DESTRUCTOR ===
//para liberar la memoria del objeto
GalleryWidget::~GalleryWidget()
{
    delete ui;
}
