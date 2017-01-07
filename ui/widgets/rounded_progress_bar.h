// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_ROUNDED_PROGRESS_BAR_H
#define INSTALLER_UI_WIDGETS_ROUNDED_PROGRESS_BAR_H

#include <QProgressBar>

namespace installer {

// Re-implement progress bar with rounded corners.
class RoundedProgressBar : public QProgressBar {
  Q_OBJECT

 public:
  explicit RoundedProgressBar(QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* event) override;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_ROUNDED_PROGRESS_BAR_H
