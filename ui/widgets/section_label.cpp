// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/section_label.h"

#include "base/file_util.h"

namespace installer {

SectionLabel::SectionLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent) {
  this->setObjectName("section_label");

  this->setStyleSheet(ReadFile(":/styles/section_label.css"));
}

}  // namespace installer