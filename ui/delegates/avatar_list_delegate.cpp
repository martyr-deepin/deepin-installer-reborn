/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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