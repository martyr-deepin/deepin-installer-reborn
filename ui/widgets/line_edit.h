// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_LINE_EDIT_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_LINE_EDIT_H

#include <QLineEdit>
class QLabel;
class QResizeEvent;

namespace ui {

// Customized line edit used in form page.
class LineEdit : public QLineEdit {
  Q_OBJECT

 public:
  LineEdit(const QString& icon, QWidget* parent = nullptr);

 protected:
  // Reset image label position.
  void resizeEvent(QResizeEvent* event) override;

 private:
  QLabel* image_label_ = nullptr;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_LINE_EDIT_H
