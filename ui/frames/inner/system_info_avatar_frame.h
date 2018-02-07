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

#ifndef INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H
#define INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H

#include <QFrame>
class QListView;

namespace installer {

class AvatarButton;
class CommentLabel;
class TitleLabel;

class SystemInfoAvatarFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoAvatarFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when an avatar is selected.
  void avatarUpdated(const QString& avatar);

  void finished();

 public slots:
  // Read default avatar and emit avatarUpdated() signal.
  void readConf();

  // Validate avatar path and write to conf file.
  void writeConf();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  AvatarButton* current_avatar_button_ = nullptr;
  QListView* list_view_ = nullptr;

 private slots:
  void onListViewPressed(const QModelIndex &index);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H
