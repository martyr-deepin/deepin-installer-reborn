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

#ifndef INSTALLER_UI_FRAMES_INNER_NEW_TABLE_LOADING_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_NEW_TABLE_LOADING_FRAME_H

#include <QFrame>

namespace installer {

class CommentLabel;
class SpinnerLabel;

// Display loading animation when creating new partition table.
class NewTableLoadingFrame : public QFrame {
  Q_OBJECT

 public:
  explicit NewTableLoadingFrame(QWidget* parent = nullptr);

 protected:
  void changeEvent(QEvent* event) override;

  // Stop spinner animation.
  void hideEvent(QHideEvent* event) override;

  // Start spinner animation.
  void showEvent(QShowEvent* event) override;

 private:
  void initUI();

  CommentLabel* comment_label_ = nullptr;
  SpinnerLabel* spinner_label_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_NEW_TABLE_LOADING_FRAME_H
