// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/first_boot_setup_window.h"

#include <QDebug>
#include <QLabel>
#include <QResizeEvent>
#include <QStackedLayout>

#include "service/settings_manager.h"
#include "ui/frames/system_info_frame.h"

namespace installer {

FirstBootSetupWindow::FirstBootSetupWindow(QWidget *parent) : QFrame(parent) {
  this->setObjectName("first_boot_setup_window");

  this->initUI();
  this->initConnections();
}

void FirstBootSetupWindow::fullscreen() {
  this->showFullScreen();

  // Read default settings.
  system_info_frame_->readConf();
}

void FirstBootSetupWindow::resizeEvent(QResizeEvent *event) {
  this->updateBackground();
  QFrame::resizeEvent(event);
}

void FirstBootSetupWindow::initConnections() {
  connect(system_info_frame_, &SystemInfoFrame::finished,
          this, &FirstBootSetupWindow::onSystemInfoFinished);
}

void FirstBootSetupWindow::initUI() {
  background_label_ = new QLabel(this);

  system_info_frame_ = new SystemInfoFrame(this);

  stacked_layout_ = new QStackedLayout(this);
  stacked_layout_->setContentsMargins(0, 0, 0, 0);
  stacked_layout_->setSpacing(0);
  stacked_layout_->addWidget(system_info_frame_);

  this->setLayout(stacked_layout_);
  this->setContentsMargins(0, 36, 0, 36);
}

void FirstBootSetupWindow::updateBackground() {
  if (!background_label_) {
    qWarning() << "background_label is not initialized!";
    return;
  }
  const QString image_path = GetWindowBackground();
  // Other platforms may have performance issue.
  const QPixmap pixmap = QPixmap(image_path).scaled(
      size(), Qt::KeepAspectRatioByExpanding);
  background_label_->setPixmap(pixmap);
  background_label_->setFixedSize(size());
}

void FirstBootSetupWindow::onSystemInfoFinished() {
  // TODO(xushaohua): Call "hooks/first_boot_setup.sh".
}

}  // namespace installer