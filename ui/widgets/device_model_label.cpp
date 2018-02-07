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

#include "ui/widgets/device_model_label.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPaintEvent>

namespace installer {

namespace {

const int kFontSize = 14;
const int kTextLeftMargin = 40;
const int kLineLeftMargin = 30;

const int kHorizontalLineHeight = 1;

}  // namespace

DeviceModelLabel::DeviceModelLabel(QWidget* parent) : QLabel(parent) {
  this->setObjectName("device_model_label");
}

DeviceModelLabel::DeviceModelLabel(const QString& text, QWidget* parent)
  : DeviceModelLabel(parent) {
  this->setText(text);
}

void DeviceModelLabel::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);

  // Draw text.
  QFont font;
  font.setPixelSize(kFontSize);
  const QFontMetrics font_metrics(font);
  const int text_width = font_metrics.width(this->text());
  const int text_height = font_metrics.height();
  const QColor text_color(QColor::fromRgb(255, 255, 255, 255));
  painter.setFont(font);
  painter.setPen(QPen(text_color));
  const QRect text_rect(kTextLeftMargin, 0, text_width, text_height);
  painter.drawText(text_rect, Qt::AlignLeft, this->text());

  // Draw horizontal line.
  const QColor line_color(QColor::fromRgb(255, 255, 255, 25));
  painter.setPen(QPen(QBrush(line_color), kHorizontalLineHeight));
  // Add left-margin
  const int line_start_x = kTextLeftMargin + text_width + kLineLeftMargin;
  const int half_height = this->height() / 2;
  painter.drawLine(line_start_x, half_height, this->width(), half_height);
}

}  // namespace installer