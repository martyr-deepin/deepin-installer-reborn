// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/partition_table_warning_frame.h"

namespace ui {

PartitionTableWarningFrame::PartitionTableWarningFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName(QStringLiteral("partition_table_warning_frame"));
}

}  // namespace ui