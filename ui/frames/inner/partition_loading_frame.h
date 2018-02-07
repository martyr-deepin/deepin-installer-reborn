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

#ifndef INSTALLER_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H

#include <QFrame>

namespace installer {

class CommentLabel;
class TitleLabel;

// Displays loading page when partition manager is scanning disk devices.
class PartitionLoadingFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionLoadingFrame(QWidget* parent = nullptr);

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H
