/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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

#ifndef DEEPIN_INSTALLER_USER_FORM_WINDOW_H
#define DEEPIN_INSTALLER_USER_FORM_WINDOW_H

#include <QWidget>

class QLabel;
class QPushButton;
class QStackedLayout;

namespace installer {

class SystemInfoFrame;

class UserFormWindow : public QWidget {
  Q_OBJECT
 public:
  explicit UserFormWindow(QWidget* parent = nullptr);
  ~UserFormWindow() override;

  // Show fullscreen.
  void fullscreen();

 protected:
  // Move close button to appropriate position when window is resized.
  void resizeEvent(QResizeEvent* event) override;

 private:

  void initConnections();
  void initPages();
  void initUI();

  void updateBackground();

  QLabel* background_label_ = nullptr;
  // All of frame pages are stored in this layout.
  // And they are referenced by id in |pages_|.
  QStackedLayout* stacked_layout_ = nullptr;

  SystemInfoFrame* system_info_frame_ = nullptr;

 private slots:
  void onUserFormFinished();
};

}  // namespace installer

#endif  // DEEPIN_INSTALLER_USER_FORM_WINDOW_H
