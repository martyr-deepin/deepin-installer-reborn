// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/partition_usage_bar.h"

#include <QPaintEvent>

namespace ui {

PartitionUsageBar::PartitionUsageBar(qint64 freespace,
                                     qint64 total,
                                     QWidget* parent)
    : QFrame(parent),
      freespace_(freespace),
      total_(total) {
  this->setObjectName(QStringLiteral("partition_usage_bar"));
}

void PartitionUsageBar::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
}

}  // namespace ui