// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_INSTALL_PROGRESS_SLIDE_FRAME_H
#define INSTALLER_INSTALL_PROGRESS_SLIDE_FRAME_H

#include <QFrame>

namespace installer {

// Displays slide in InstallProgressFrame
class InstallProgressSlideFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallProgressSlideFrame(QWidget* parent = nullptr);

  // Start/stop slide show.
  void startSlide();
  void stopSlide();

 private:
  void initConnections();
  void initUI();

  QString locale_;

 private slots:
  void onLanguageUpdated(const QString& locale);
};

}  // namespace installer

#endif  // INSTALLER_INSTALL_PROGRESS_SLIDE_FRAME_H
