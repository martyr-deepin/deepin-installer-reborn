// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INSTALL_FAILED_FRAME_H
#define INSTALLER_UI_FRAMES_INSTALL_FAILED_FRAME_H

#include <QFrame>
class QLabel;
class QPushButton;

namespace installer {

class CommentLabel;
class NavButton;
class QRWidget;
class TitleLabel;

// Displays error message and a QR image
class InstallFailedFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallFailedFrame(QWidget* parent = nullptr);

  // Reload error message.
  // |msg| is plain text. And |encoded_msg| is based64 encoded |msg|, with
  // prefix of feedback server domain.
  void updateErrorMessage(const QString& msg, const QString& encoded_msg);

 signals:
  // Emitted when reboot button is clocked.
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  NavButton* reboot_button_ = nullptr;
  QRWidget* qr_widget_ = nullptr;
  QLabel* content_label_ = nullptr;
  QPushButton* control_button_ = nullptr;

 private slots:
  void onControlButtonClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INSTALL_FAILED_FRAME_H
