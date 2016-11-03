// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/title_label.h"

#include "base/file_util.h"

namespace installer {

TitleLabel::TitleLabel(const QString& text, QWidget* parent)
  : QLabel(text, parent) {
  this->setAlignment(Qt::AlignHCenter);
  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  this->setStyleSheet(ReadFile(":/styles/title_label.css"));
}

}  // namespace installer