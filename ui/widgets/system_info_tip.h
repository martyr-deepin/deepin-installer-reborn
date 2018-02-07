/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
