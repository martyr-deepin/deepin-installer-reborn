// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/avatar_list_delegate.h"

#include <QDebug>
#include <QPainter>
#include <QStyle>

namespace installer {

namespace {

// TODO(xushaohua): Defines a constant value.
// Duplicated with AvatarButton.
const int kIconSize = 80;

}  // namespace

AvatarListDelegate::AvatarListDelegate(QWidget* parent)
    : QStyledItemDelegate(parent) {
  this->setObjectName("avatar_list_delegate");
}

void
AvatarListDelegate::paint(QPainter* painter,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const {
  painter->save();
  const QRect& rect(option.rect);
  const QString icon_path = index.model()->data(index).toString();
  if (!icon_path.isEmpty()) {
    const QRect ellipse_rect(rect.x() + (rect.width() - kIconSize) / 2,
                             rect.y() + (rect.height() - kIconSize) / 2,
                             kIconSize, kIconSize);
    QPainterPath path;
    path.addEllipse(ellipse_rect);
    painter->setRenderHint(QPainter::Antialiasing);
    // Scale image.
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setClipPath(path);

    const QImage image(icon_path);
    painter->drawImage(ellipse_rect, image);
  } else {
    qWarning() << "Avatar icon is empty!";
  }

  painter->restore();
}

}  // namespace installer