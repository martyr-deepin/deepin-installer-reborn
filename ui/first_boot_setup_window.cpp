// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/first_boot_setup_window.h"

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QResizeEvent>
#include <QStackedLayout>
#include <QThread>

#include "service/first_boot_hook_worker.h"
#include "service/settings_manager.h"
#include "ui/frames/first_boot_loading_frame.h"
#include "ui/frames/system_info_frame.h"

namespace installer {

FirstBootSetupWindow::FirstBootSetupWindow(QWidget *parent)
    : QFrame(parent),
      hook_worker_thread_(new QThread(this)),
      hook_worker_(new FirstBootHookWorker()) {
  this->setObjectName("first_boot_setup_window");

  hook_worker_thread_->start();
  hook_worker_->moveToThread(hook_worker_thread_);

  this->initUI();
  this->initConnections();

  // Read default settings.
  system_info_frame_->readConf();
}

FirstBootSetupWindow::~FirstBootSetupWindow() {
  qCritical() << "destructor";
  hook_worker_thread_->quit();
  hook_worker_thread_->wait(3);
}

void FirstBootSetupWindow::fullscreen() {
  this->showFullScreen();
}

void FirstBootSetupWindow::resizeEvent(QResizeEvent *event) {
  this->updateBackground();
  QFrame::resizeEvent(event);
}

void FirstBootSetupWindow::initConnections() {
  connect(system_info_frame_, &SystemInfoFrame::finished,
          this, &FirstBootSetupWindow::onSystemInfoFinished);

  connect(hook_worker_, &FirstBootHookWorker::hookFinished,
          this, &FirstBootSetupWindow::onHookFinished);
}

void FirstBootSetupWindow::initUI() {
  background_label_ = new QLabel(this);

  system_info_frame_ = new SystemInfoFrame(this);
  loading_frame_ = new FirstBootLoadingFrame(this);

  stacked_layout_ = new QStackedLayout(this);
  stacked_layout_->setContentsMargins(0, 0, 0, 0);
  stacked_layout_->setSpacing(0);
  stacked_layout_->addWidget(system_info_frame_);
  stacked_layout_->addWidget(loading_frame_);

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

void FirstBootSetupWindow::onHookFinished(bool ok) {
  if (!ok) {
    qCritical() << "First boot hook failed!";
  }

  // TODO(xushaohua): Remove first-boot-setup itself.
  qApp->exit(1);
}

void FirstBootSetupWindow::onSystemInfoFinished() {
  // Display loading frame.
  stacked_layout_->setCurrentWidget(loading_frame_);

  emit hook_worker_->startHook();
}

}  // namespace installer