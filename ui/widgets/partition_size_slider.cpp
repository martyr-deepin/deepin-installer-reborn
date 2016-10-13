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

PartitionSizeSlider::PartitionSizeSlider(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("partition_size_slider"));

  this->initUI();
  this->initConnection();
}

qint64 PartitionSizeSlider::value() {
  return slider_->value() * kMebiByte;
}

void PartitionSizeSlider::setMaximum(qint64 size) {
  const int mebi_size = static_cast<int>(size / kMebiByte);
  slider_->setMaximum(mebi_size);
  slider_->setValue(mebi_size);
  int_validator_->setRange(slider_->minimum(), mebi_size);
}

void PartitionSizeSlider::initConnection() {
  connect(slider_, &QSlider::valueChanged,
          this, &PartitionSizeSlider::onSliderValueChanged);
  connect(editor_, &QLineEdit::textChanged,
          this, &PartitionSizeSlider::onEditorTextChanged);
}

void PartitionSizeSlider::initUI() {
  slider_ = new QSlider(Qt::Horizontal);
  slider_->setMinimum(0);
  editor_ = new QLineEdit();
  int_validator_ = new QIntValidator(editor_);
  editor_->setValidator(int_validator_);

  QLabel* size_label = new QLabel("Mib");
  size_label->setObjectName(QStringLiteral("size_label"));

  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(slider_);
  layout->addWidget(editor_);
  layout->addWidget(size_label);
  this->setLayout(layout);
  this->setStyleSheet(
      ReadTextFileContent(":/styles/partition_size_slider.css"));
}

void PartitionSizeSlider::onEditorTextChanged(const QString& text) {
  bool ok;
  const int value = text.toInt(&ok);
  if (ok) {
    slider_->setValue(value);
  }
}

void PartitionSizeSlider::onSliderValueChanged(int value) {
  editor_->setText(QString::number(value));
}

}  // namespace installer