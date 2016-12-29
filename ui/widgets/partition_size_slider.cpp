// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/partition_size_slider.h"

#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>

#include "base/file_util.h"
#include "partman/structs.h"

namespace installer {

PartitionSizeSlider::PartitionSizeSlider(QWidget* parent)
    : QFrame(parent),
      maximum_size_(0),
      minimum_size_(0) {
  this->setObjectName("partition_size_slider");

  this->initUI();
  this->initConnection();
}

qint64 PartitionSizeSlider::value() {
  // Keep maximum value unchanged.
  if (slider_->value() == slider_->maximum()) {
    return maximum_size_;
  } else {
    return slider_->value() * kMebiByte;
  }
}

void PartitionSizeSlider::setMaximum(qint64 maximum_size) {
  Q_ASSERT(maximum_size >= 0);
  maximum_size_ = maximum_size;
  // First convert bytes to mebibytes.
  const int mebi_size = static_cast<int>(maximum_size / kMebiByte);
  slider_->setMaximum(mebi_size);
  slider_->setValue(mebi_size);
  int_validator_->setRange(slider_->minimum(), mebi_size);
}

void PartitionSizeSlider::setMinimum(qint64 minimum_size) {
  Q_ASSERT(minimum_size >= 0);
  // Make sure that |minimum_size| is in range.
  const qint64 min_size = qMin(minimum_size, maximum_size_);
  minimum_size_ = min_size;
  // Convert bytes to mebibytes.
  const int mebi_size = static_cast<int>(min_size / kMebiByte);
  slider_->setMinimum(mebi_size);
  int_validator_->setRange(mebi_size, slider_->maximum());
}

void PartitionSizeSlider::setValue(qint64 size) {
  Q_ASSERT(size >= minimum_size_);
  Q_ASSERT(size <= maximum_size_);
  qint64 real_size = qMin(size, maximum_size_);
  real_size = qMax(size, minimum_size_);
  // Convert to mebibytes.
  const int mebi_size = static_cast<int>(real_size / kMebiByte);
  slider_->setValue(mebi_size);
}

void PartitionSizeSlider::initConnection() {
  connect(slider_, &QSlider::valueChanged,
          this, &PartitionSizeSlider::onSliderValueChanged);
  connect(editor_, &QLineEdit::textChanged,
          this, &PartitionSizeSlider::onEditorTextChanged);
}

void PartitionSizeSlider::initUI() {
  slider_ = new QSlider(Qt::Horizontal);
  slider_->setObjectName("slider");
  slider_->setMinimum(0);

  editor_ = new QLineEdit();
  editor_->setObjectName("editor");
  int_validator_ = new QIntValidator(editor_);
  int_validator_->setRange(0, 1);
  editor_->setValidator(int_validator_);
  editor_->setFixedWidth(68);

  QLabel* size_label = new QLabel("MiB");
  size_label->setObjectName("size_label");

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(slider_);
  layout->addSpacing(8);
  layout->addWidget(editor_);
  layout->addWidget(size_label);
  this->setLayout(layout);
  this->setContentsMargins(10, 0, 10, 0);
  // Same as TableComboBox
  this->setFixedSize(280, 36);
  this->setStyleSheet(ReadFile(":/styles/partition_size_slider.css"));
}

void PartitionSizeSlider::onEditorTextChanged(const QString& text) {
  bool ok;
  const int value = text.toInt(&ok);
  if (ok) {
    // Block value-changed signal of slider to hold cursor position in editor.
    slider_->blockSignals(true);
    slider_->setValue(value);
    slider_->blockSignals(false);
  }
}

void PartitionSizeSlider::onSliderValueChanged(int value) {
  editor_->setText(QString::number(value));
}

}  // namespace installer