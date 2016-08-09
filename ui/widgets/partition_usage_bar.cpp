// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/partition_usage_bar.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

namespace ui {

PartitionUsageBar::PartitionUsageBar(qint64 freespace,
                                     qint64 total,
                                     QWidget* parent)
    : QFrame(parent),
      freespace_(freespace),
      total_(total) {
  this->setObjectName(QStringLiteral("partition_usage_bar"));
  this->setFixedSize(80, 10);
}

void PartitionUsageBar::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);

  const QColor base_color(220, 220, 220, 100);
  painter.fillRect(0, 0, this->width(), this->height(), base_color);

  const QColor fill_color(44, 167, 248, 255);
  if (total_ > 0 && freespace_ > 0) {
    const int usage =
        static_cast<int>(freespace_ * 1.0 / total_ * this->width());
    painter.fillRect(0, 0, usage, this->height(), fill_color);
  }
}

}  // namespace ui