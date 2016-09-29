// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/subhead_label.h"

#include "base/file_util.h"

namespace installer {

SubheadLabel::SubheadLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent) {
  this->setObjectName(QStringLiteral("subhead_label"));

  this->setAlignment(Qt::AlignCenter);
  this->setWordWrap(true);
  this->setStyleSheet(ReadTextFileContent(":/styles/subhead_label.css"));
}

}  // namespace installer