// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "misc/oem/section_label.h"

#include "base/file_util.h"

namespace installer {

SectionLabel::SectionLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent) {
  this->setObjectName(QStringLiteral("section_label"));

  this->setStyleSheet(ReadFile(":/oem_styles/section_label.css"));
}

}  // namespace installer