// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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