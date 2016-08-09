// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_PARTITION_USAGE_BAR_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_PARTITION_USAGE_BAR_H

#include <QFrame>
class QPaintEvent;

namespace ui {

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

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_PARTITION_USAGE_BAR_H
