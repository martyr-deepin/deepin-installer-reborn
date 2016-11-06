// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H
#define INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H

#include <QFrame>
class QListView;

namespace installer {

class AvatarButton;
class IconButton;

class SystemInfoAvatarFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoAvatarFrame(QWidget* parent = nullptr);

  void autoConf();

 signals:
  // Emitted when an avatar is selected.
  void avatarUpdated(const QString& avatar);

  void finished();

  // Emitted when timezone button is clicked.
  void timezoneClicked();

 public slots:
  // Update text in timezone button.
  void updateTimezone(const QString& timezone);

 private:
  void initConnections();
  void initUI();

  AvatarButton* current_avatar_button_ = nullptr;
  IconButton* timezone_button_ = nullptr;
  QListView* list_view_ = nullptr;

 private slots:
  void onListViewPressed(const QModelIndex &index);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H
