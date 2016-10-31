// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H
#define INSTALLER_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H

#include <QPushButton>
class QLabel;
class QPaintEvent;

#include "partman/partition.h"

namespace installer {

class SimplePartitionButton : public QPushButton {
  Q_OBJECT

 public:
  SimplePartitionButton(const Partition& partition, QWidget* parent = nullptr);

 protected:
  // Override these two event handlers to implements hover effect.
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  const Partition& partition_;
  bool mouse_hovered_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H
