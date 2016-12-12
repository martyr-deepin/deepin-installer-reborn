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

  // Get number of rows in model.
  const int row_count = index.model()->rowCount(QModelIndex());

  const bool is_first_row = (index.row() == 0);
  const bool is_last_row = (index.row() + 1 == row_count);

  if (option.state & QStyle::State_MouseOver) {
    // Draw background color of selected item, no matter it is active or not.
    const QColor selected_color(44, 167, 248, 200);
    const QBrush background_brush(selected_color);
    const int x = rect.x();
    const int y = rect.y();
    const int width = rect.width();
    const int height = rect.height();
    if (is_first_row) {

      // First draw background with round corner.
      QPainterPath background_path;
      background_path.moveTo(x + width, y + kBorderRadius);
      background_path.arcTo(x + width - kBorderDiameter, y,
                            kBorderDiameter, kBorderDiameter,
                            0.0, 90.0);
      background_path.lineTo(x + kBorderRadius, y);
      background_path.arcTo(x, y, kBorderDiameter, kBorderDiameter, 90.0, 90.0);
      background_path.lineTo(x, y + height);
      background_path.lineTo(x + width, y + height);
      background_path.closeSubpath();
      painter->fillPath(background_path, background_brush);

    } else if (is_last_row) {
      QPainterPath background_path;
      background_path.moveTo(x + width, y);
      background_path.lineTo(x, y);
      background_path.lineTo(x, y + height - kBorderRadius);
      background_path.arcTo(x, y + height - kBorderDiameter,
                            kBorderDiameter, kBorderDiameter,
                            180.0, 90.0);
      background_path.lineTo(x + width - kBorderRadius, y + height);
      background_path.arcTo(x + width - kBorderDiameter,
                            y + height - kBorderDiameter,
                            kBorderDiameter, kBorderDiameter,
                            270.0, 90.0);
      background_path.closeSubpath();
      painter->fillPath(background_path, background_brush);
    } else {
      painter->fillRect(rect, background_brush);
    }
  }

  // Draw text
  QColor text_color(255, 255, 255, 200);
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