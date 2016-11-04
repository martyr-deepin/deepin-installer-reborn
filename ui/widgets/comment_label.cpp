// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/comment_label.h"

#include "base/file_util.h"

namespace installer {

CommentLabel::CommentLabel(const QString& text, QWidget* parent)
  : QLabel(text, parent) {
  this->setObjectName("comment_label");

  this->setStyleSheet(ReadFile(":/styles/comment_label.css"));
  this->setFixedWidth(540);
  this->setWordWrap(true);
}

}  // namespace installer