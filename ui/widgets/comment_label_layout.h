// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_COMMENT_LABEL_LAYOUT_H
#define INSTALLER_COMMENT_LABEL_LAYOUT_H

#include <QHBoxLayout>

namespace installer {

// A layout wrapper of CommentLabel.
class CommentLabelLayout : public QHBoxLayout {
  Q_OBJECT

 public:
  CommentLabelLayout(const QString& text, QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_COMMENT_LABEL_LAYOUT_H
