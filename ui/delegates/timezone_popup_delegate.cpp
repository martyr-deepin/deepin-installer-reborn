// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/timezone_popup_delegate.h"

#include <QPainter>

namespace installer {

namespace {

const int kBorderRadius = 5;
const int kBorderDiameter = kBorderRadius * 2;

}  // namespace

TimezonePopupDelegate::TimezonePopupDelegate(QWidget* parent)
    : QStyledItemDelegate(parent) {
  this->setObjectName("timezone_popup_delegate");
}

void TimezonePopupDelegate::paint(QPainter* painter,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const {
  painter->save();

  const QRect& rect(option.rect);

  if (option.state & QStyle::State_MouseOver) {
    // Draw background color of selected item, no matter it is active or not.
    // #2ca7f8
    const QColor selected_color = QColor::fromRgb(44, 167, 248);
    painter->fillRect(rect, QBrush(selected_color));
  }

  // Draw text. Default color is #303030.
  QColor text_color = QColor::fromRgb(48, 48, 48);
  if (option.state & QStyle::State_MouseOver) {
    text_color = Qt::white;
  }
  painter->setPen(QPen(text_color));
  const QString text = index.model()->data(index, Qt::DisplayRole).toString();

  // Text alignment is center.
  painter->drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, text);

  painter->restore();
}

}  // namespace installer