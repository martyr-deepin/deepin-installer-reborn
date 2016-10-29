// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/comment_label.h"

#include "base/file_util.h"

namespace installer {

CommentLabel::CommentLabel(const QString& text, QWidget* parent)
  : QLabel(text, parent) {
  this->setObjectName(QStringLiteral("comment_label"));
  this->setWordWrap(true);
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//  QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//  size_policy.setVerticalStretch(1);
//  this->setSizePolicy(size_policy);
  this->setStyleSheet(ReadTextFileContent(":/styles/comment_label.css"));
}

}  // namespace installer