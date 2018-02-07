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

#include "ui/widgets/rounded_progress_bar.h"

#include <QPainter>
#include <QPainterPath>

namespace installer {

RoundedProgressBar::RoundedProgressBar(QWidget* parent) : QProgressBar(parent) {
  this->setObjectName("rounded_progress_bar");

  // Hide text.
  this->setTextVisible(false);
}

void RoundedProgressBar::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  const int width = this->width();
  const int height = this->height();
  // TODO(xushaohua): Radius border-radius and background color from QStyle.
  // Border radius is half of progress bar height.
  const int radius = height / 2;
  QPainterPath base_path;
  // base_color == "rgba(255, 255, 255, 0.1)"
  const QColor base_color(255, 255, 255, 25);
  base_path.addRoundedRect(0, 0, width, height, radius, radius);
  painter.fillPath(base_path, base_color);

  QPainterPath chunk_path;
  const int chunk_width = this->value() * width / this->maximum();
  // chunk_color == "#2ca7f8"
  const QColor chunk_color(44, 167, 248);
  chunk_path.addRoundedRect(0, 0, chunk_width, height, radius, radius);
  painter.fillPath(chunk_path, chunk_color);
}

}  // namespace installer