// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_SYSTEM_INFO_TIP_H
#define INSTALLER_UI_WIDGETS_SYSTEM_INFO_TIP_H

#include <QFrame>
class QLabel;
class QPaintEvent;

namespace installer {

// Display tooltip text in SystemInfoFormFrame
class SystemInfoTip : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoTip(QWidget* parent = nullptr);

  // Update tooltip text to |text|.
  void setText(const QString& text);

  // Show tooltip at bottom of |widget|.
  void showBottom(QWidget* widget);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  void initUI();

  QLabel* label_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_SYSTEM_INFO_TIP_H
