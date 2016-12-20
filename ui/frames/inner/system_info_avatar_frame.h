// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
