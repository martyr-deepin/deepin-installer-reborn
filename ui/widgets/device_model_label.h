// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_DEVICE_MODEL_LABEL_H
#define INSTALLER_UI_WIDGETS_DEVICE_MODEL_LABEL_H

#include <QLabel>
class QPaintEvent;

namespace installer {

// Draw model name and a horizontal line. Used in SimplePartitionFrame.
class DeviceModelLabel : public QLabel {
  Q_OBJECT

 public:
  explicit DeviceModelLabel(QWidget* parent = nullptr);
  DeviceModelLabel(const QString& text, QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* event) override;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_DEVICE_MODEL_LABEL_H
