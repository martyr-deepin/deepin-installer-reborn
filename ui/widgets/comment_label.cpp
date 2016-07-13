// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/comment_label.h"

#include "base/file_util.h"

namespace ui {

CommentLabel::CommentLabel(const QString& text, QWidget* parent)
  : QLabel(text, parent) {
  this->setWordWrap(true);
  this->setAlignment(Qt::AlignHCenter);
  this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  this->setStyleSheet(
      base::ReadTextFileContent(QStringLiteral(":/styles/comment_label.css")));
}

}  // namespace ui