#include "PictureDelegate.h"
#include <QPainter>

const unsigned int BANNER_HEIGHT = 20;
const unsigned int BANNER_COLOR = 0x303030;
const unsigned int BANNER_ALPHA = 200;
const unsigned int BANNER_TEXT_COLOR = 0xffffff;
const unsigned int HIGHLIGHT_ALPHA = 100;

PictureDelegate::PictureDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}


void PictureDelegate::paint(QPainter* painter,
                            const QStyleOptionViewItem& option,
                            const QModelIndex& index) const
{
    painter->save();

    // obtener miniatura
    QPixmap pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();

    // miniatura
    painter->drawPixmap(option.rect.x(), option.rect.y(), pixmap);

    // banda gris
    QRect bannerRect(option.rect.x(),
                     option.rect.y(),
                     pixmap.width(),
                     BANNER_HEIGHT);

    QColor bannerColor(BANNER_COLOR);
    bannerColor.setAlpha(BANNER_ALPHA);
    painter->fillRect(bannerRect, bannerColor);

    // nombre del archivo
    QString filename = index.model()->data(index, Qt::DisplayRole).toString();
    painter->setPen(BANNER_TEXT_COLOR);
    painter->drawText(bannerRect, Qt::AlignCenter, filename);

    // highlight si está seleccionado
    if (option.state.testFlag(QStyle::State_Selected)) {
        QColor selectedColor = option.palette.highlight().color();
        selectedColor.setAlpha(HIGHLIGHT_ALPHA);
        painter->fillRect(option.rect, selectedColor);
    }

    painter->restore();
}

QSize PictureDelegate::sizeHint(const QStyleOptionViewItem&,
                                const QModelIndex& index) const
{
    const QPixmap& pixmap =
        index.model()->data(index, Qt::DecorationRole).value<QPixmap>();

    return pixmap.size();
}
