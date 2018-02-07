/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

 signals:
  // Emitted when slide |value| is updated by user or by program.
  void valueChanged(qint64 value);

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
