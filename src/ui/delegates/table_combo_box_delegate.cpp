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

#include "ui/delegates/table_combo_box_delegate.h"
#include "ui/utils/widget_util.h"

#include <QDebug>
#include <QPainter>
#include <QStyle>

namespace installer {

namespace {

// Left margin of text content.
const int kTextLeftMargin = 25;

// Right margin of selected item, used to locate background image.
const int kSelectedRightMargin = 20;

// Size of bottom border.
const int kBorderBottom = 1;

}  // namespace

TableComboBoxDelegate::TableComboBoxDelegate(QWidget* parent)
    : QStyledItemDelegate(parent) {
  this->setObjectName("table_combo_box_delegate");
}

void TableComboBoxDelegate::paint(QPainter* painter,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const {
  painter->save();

  const QRect& rect(option.rect);
  const QRect background_rect(rect.x(), rect.y(), rect.width(),
                              rect.height() - kBorderBottom);
  if (option.state & QStyle::State_Selected) {
    // Draw background image of selected item.
    const QPixmap pixmap = std::move(installer::renderPixmap(":/images/select.svg"));
    const int x = rect.x() + rect.width() - pixmap.width() -
                  kSelectedRightMargin;
    const int y = rect.y() + (rect.height() - pixmap.height()) / 2;
    const QRect pixmap_rect(x, y, pixmap.width(), pixmap.height());
    painter->drawPixmap(pixmap_rect, pixmap);

    // Draw background color of selected item, no matter it is active or not.
    const QColor selected_color(44, 167, 248);
    const QBrush background_brush(selected_color);
    painter->fillRect(background_rect, background_brush);
  }  else if (option.state & QStyle::State_MouseOver) {
    // Draw background color when mouse is hover
    // Background color: #2CA7F8
    const QColor selected_color(44, 167, 248);
    const QBrush background_brush(selected_color);
    painter->fillRect(background_rect, background_brush);
  }

  // Draw text
  QColor text_color(0, 0, 0, 255);
  if ((option.state & QStyle::State_MouseOver) ||
      (option.state & QStyle::State_Selected)) {
    text_color = Qt::white;
  }
  painter->setPen(QPen(text_color));
  const QString text = index.model()->data(index, Qt::DisplayRole).toString();
  const QRect text_rect(rect.x() + kTextLeftMargin, rect.y(),
                        rect.width(), rect.height());
  painter->drawText(text_rect, Qt::AlignLeft | Qt::AlignVCenter, text);

  // Draw bottom border of item without last one.
  // NOTE(xushaohua): Do not draw separator any more.
//  if (index.row() + 1 != index.model()->rowCount(index)) {
//    const QColor border_color(0, 0, 0, 20);
//    QPen border_pen(border_color);
//    border_pen.setWidth(kBorderBottom);
//    painter->setPen(border_pen);
//    const QLine border_line(option.rect.bottomLeft(), option.rect.bottomRight());
//    painter->drawLine(border_line);
//  }

  painter->restore();
}

}  // namespace installer
