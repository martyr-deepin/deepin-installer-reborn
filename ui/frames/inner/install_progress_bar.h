// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_INSTALL_PROGRESS_BAR_H
#define INSTALLER_INSTALL_PROGRESS_BAR_H

#include <QFrame>

namespace installer {

// Displays a progress bar and a tooltip in InstallProgressValue
class InstallProgressBar : public QFrame {
  Q_OBJECT

 public:
  explicit InstallProgressBar(QWidget* parent = nullptr);

 public slots:
  // Update progress value
  int progress() const { return progress_; }
  void setProgress(int progress);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  int progress_;
};

}  // namespace installer

#endif  // INSTALLER_INSTALL_PROGRESS_BAR_H
