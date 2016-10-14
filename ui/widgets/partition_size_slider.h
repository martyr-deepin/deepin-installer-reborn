// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_PARTITION_SIZE_SLIDER_H
#define INSTALLER_UI_PARTITION_SIZE_SLIDER_H

#include <QFrame>
class QIntValidator;
class QLineEdit;
class QSlider;

namespace installer {

class PartitionSizeSlider : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionSizeSlider(QWidget* parent = nullptr);

  // Get current partition size, in byte.
  qint64 value();

  // Set maximum partition size, in byte.
  void setMaximum(qint64 size);

 private:
  void initConnection();
  void initUI();

  QSlider* slider_ = nullptr;
  QLineEdit* editor_ = nullptr;
  QIntValidator* int_validator_ = nullptr;

  qint64 maximum_value_;

 private:
  void onEditorTextChanged(const QString& text);
  void onSliderValueChanged(int value);
};

}  // namespace installer

#endif  // INSTALLER_UI_PARTITION_SIZE_SLIDER_H
