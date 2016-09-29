// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_PARTITION_USAGE_BAR_H
#define INSTALLER_UI_WIDGETS_PARTITION_USAGE_BAR_H

#include <QFrame>
class QPaintEvent;

namespace installer {

// Displays partition capacity and used size.
// Used in SimplePartitionButton.
class PartitionUsageBar : public QFrame {
  Q_OBJECT

 public:
  PartitionUsageBar(qint64 freespace, qint64 total, QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  qint64 freespace_;
  qint64 total_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_PARTITION_USAGE_BAR_H
