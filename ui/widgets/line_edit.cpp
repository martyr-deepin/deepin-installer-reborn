// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/line_edit.h"

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

#include "base/file_util.h"

namespace ui {

namespace {

const int kImageLabelSize = 32;
const int kFixedHeight = 48;
const int kImageLabelOffsetY = 8;

}  // namespace

LineEdit::LineEdit(const QString& icon, QWidget* parent)
    : QLineEdit(parent) {
  this->setObjectName(QStringLiteral("line_edit"));

  this->setStyleSheet(base::ReadTextFileContent(":/styles/line_edit.css"));
  this->setFixedHeight(kFixedHeight);

  image_label_ = new QLabel(this);
  image_label_->setPixmap(QPixmap(icon));
  image_label_->setFixedSize(kImageLabelSize, kImageLabelSize);
  image_label_->move(0, kImageLabelOffsetY);
}

void LineEdit::resizeEvent(QResizeEvent* event) {
  image_label_->move(0, kImageLabelOffsetY);
  QLineEdit::resizeEvent(event);
}

}  // namespace ui