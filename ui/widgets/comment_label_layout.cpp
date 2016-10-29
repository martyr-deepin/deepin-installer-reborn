// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/comment_label_layout.h"

#include "ui/widgets/comment_label.h"

namespace installer {

CommentLabelLayout::CommentLabelLayout(const QString& text, QWidget* parent)
  : QHBoxLayout(parent) {
  CommentLabel* label = new CommentLabel(text);
  this->setSpacing(0);
  this->setContentsMargins(0, 0, 0, 0);
  this->addWidget(label);
}

}  // namespace installer