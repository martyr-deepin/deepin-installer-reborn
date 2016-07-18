// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/list_title_label.h"

#include "base/file_util.h"

namespace ui {

ListTitleLabel::ListTitleLabel(const QString& text, QWidget* parent)
  : QLabel(text, parent){
  this->setObjectName(QStringLiteral("list_title_label"));
  this->setStyleSheet(
      base::ReadTextFileContent(":/styles/list_title_label.css"));
  this->setAlignment(Qt::AlignLeft);
  this->setWordWrap(true);
}

}  // namespace ui