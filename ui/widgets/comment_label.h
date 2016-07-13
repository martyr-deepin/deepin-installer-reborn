// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_COMMENT_LABEL_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_COMMENT_LABEL_H

#include <QLabel>

namespace ui {

// CommentLabel displays description text below TitleLabel.
class CommentLabel : public QLabel {
  Q_OBJECT

 public:
  CommentLabel(const QString& text, QWidget* parent = nullptr);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_COMMENT_LABEL_H
