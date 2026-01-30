#ifndef PICTUREDELEGATE_H
#define PICTUREDELEGATE_H

#include "gallery-core_global.h"
#include <QStyledItemDelegate>
#include <QPainter>

class GALLERY_CORE_EXPORT PictureDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit PictureDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};

#endif // PICTUREDELEGATE_H
