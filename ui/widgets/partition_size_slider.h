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

// A slider to display partition size. A line edit is also shown at right
// side of slider.
class PartitionSizeSlider : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionSizeSlider(QWidget* parent = nullptr);

  // Get current partition size, in bytes.
  qint64 value();

  // Get maximum size, in bytes.
  qint64 maximum() const { return maximum_size_; }

  // Get minimum size, in bytes.
  qint64 minimum() const { return minimum_size_; }

 public slots:
  // Set maximum partition size, in bytes.
  void setMaximum(qint64 maximum_size);

  // Set minimum value of slider, in bytes.
  // |minimum_size| shall be in range [0, maximum_size_].
  void setMinimum(qint64 minimum_size);

  // Set value of slider to |size|, in bytes.
  // |size| shall be in range [minimum_size_, maximum_size_].
  void setValue(qint64 size);

 private:
  void initConnection();
  void initUI();

  QSlider* slider_ = nullptr;
  QLineEdit* editor_ = nullptr;
  QIntValidator* int_validator_ = nullptr;

  qint64 maximum_size_;
  qint64 minimum_size_;

 private:
  void onEditorTextChanged(const QString& text);
  void onSliderValueChanged(int value);
};

}  // namespace installer

#endif  // INSTALLER_UI_PARTITION_SIZE_SLIDER_H
