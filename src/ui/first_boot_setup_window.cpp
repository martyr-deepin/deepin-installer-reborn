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

#include "ui/first_boot_setup_window.h"

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QResizeEvent>
#include <QStackedLayout>
#include <QThread>

#include "base/thread_util.h"
#include "service/first_boot_hook_worker.h"
#include "service/power_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "third_party/global_shortcut/global_shortcut.h"
#include "ui/frames/first_boot_loading_frame.h"
#include "ui/frames/system_info_frame.h"
#include "ui/frames/timezone_frame.h"
#include "ui/utils/widget_util.h"
#include "ui/xrandr/multi_head_manager.h"

#include "ui/frames/language_frame.h"
#include "ui/frames/networkframe.h"
#include "ui/frames/control_platform_frame.h"

namespace installer {

FirstBootSetupWindow::FirstBootSetupWindow(QWidget *parent)
    : QFrame(parent),
      hook_worker_thread_(new QThread(this)),
      hook_worker_(new FirstBootHookWorker()) {
  this->setObjectName("first_boot_setup_window");

  hook_worker_thread_->start();
  hook_worker_->moveToThread(hook_worker_thread_);

  this->initUI();
  this->registerShortcut();
  this->initConnections();

  // Read default settings.
  language_frame_->readConf();
  system_info_frame_->readConf();
  timezone_frame_->readConf();

    if (GetSettingsBool(kSkipSelectLanguagePageOnFirstBoot)) {
        language_frame_->writeConf();
        onLanguageSelected();
    }
}

FirstBootSetupWindow::~FirstBootSetupWindow() {
  QuitThread(hook_worker_thread_);
}

void FirstBootSetupWindow::fullscreen() {
  ShowFullscreen(this);
}

void FirstBootSetupWindow::resizeEvent(QResizeEvent *event) {
  this->updateBackground();
  QFrame::resizeEvent(event);
}

void FirstBootSetupWindow::initConnections() {
    connect(language_frame_, &LanguageFrame::finished, this,
            &FirstBootSetupWindow::onLanguageSelected);
    connect(system_info_frame_, &SystemInfoFrame::finished,
            this, &FirstBootSetupWindow::onSystemInfoFinished);
    connect(network_frame_, &NetworkFrame::requestNext,
            this, &FirstBootSetupWindow::onNetworkFinished);
    connect(control_platform_frame_, &ControlPlatformFrame::requestFinished, this,
            &FirstBootSetupWindow::onControlPlatformFinished);
    connect(timezone_frame_, &TimezoneFrame::finished,
            this, &FirstBootSetupWindow::onTimezoneFinished);
    connect(hook_worker_, &FirstBootHookWorker::hookFinished,
            this, &FirstBootSetupWindow::onHookFinished);

    connect(monitor_mode_shortcut_, &GlobalShortcut::activated,
            multi_head_manager_, &MultiHeadManager::switchXRandRMode);
    connect(multi_head_manager_, &MultiHeadManager::primaryScreenChanged,
            this, &FirstBootSetupWindow::onPrimaryScreenChanged);
}

void FirstBootSetupWindow::initUI() {
  background_label_ = new QLabel(this);
  language_frame_ = new LanguageFrame(this);
  system_info_frame_ = new SystemInfoFrame(this);
  network_frame_ = new NetworkFrame(this);
  timezone_frame_ = new TimezoneFrame(this);
  loading_frame_ = new FirstBootLoadingFrame(this);
  control_platform_frame_ = new ControlPlatformFrame(this);

  stacked_layout_ = new QStackedLayout(this);
  stacked_layout_->setContentsMargins(0, 0, 0, 0);
  stacked_layout_->setSpacing(0);
  stacked_layout_->addWidget(language_frame_);
  stacked_layout_->addWidget(system_info_frame_);
  stacked_layout_->addWidget(network_frame_);
  stacked_layout_->addWidget(control_platform_frame_);
  stacked_layout_->addWidget(timezone_frame_);
  stacked_layout_->addWidget(loading_frame_);

  this->setLayout(stacked_layout_);
  this->setContentsMargins(0, 36, 0, 36);
}

void FirstBootSetupWindow::registerShortcut() {
  // Note(xushaohua): Super key is named Meta key in Qt namespace.
  monitor_mode_shortcut_ = new GlobalShortcut(QKeySequence("Meta+P"), this);
  if (!monitor_mode_shortcut_->registerNow()) {
    qWarning() << "Failed to register global shortcut of Meta+P"
               << "Fallback to Ctrl+Alt+P";
    delete monitor_mode_shortcut_;
    monitor_mode_shortcut_ = new GlobalShortcut(QKeySequence("Ctrl+Alt+P"),
                                                this);
    if (!monitor_mode_shortcut_->registerNow()) {
      qWarning() << "Failed to register global shortcut of Ctrl+Alt+P";
    }
  }

  multi_head_manager_ = new MultiHeadManager(this);
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

  // Reboot system now.
  // TODO(xushaohua): call systemd-firstboot instead.
  if (!RebootSystemWithMagicKey()) {
    RebootSystem();
  }
}

void FirstBootSetupWindow::onPrimaryScreenChanged(const QRect& geometry) {
  qDebug() << "on primary screen changed" << geometry;
  this->move(geometry.topLeft());
  this->setFixedSize(geometry.size());
}

void FirstBootSetupWindow::onLanguageSelected()
{
    stacked_layout_->setCurrentWidget(system_info_frame_);
}

void FirstBootSetupWindow::onSystemInfoFinished() {
  if (GetSettingsBool(kSkipNetworkPage)) {
    this->onNetworkFinished();
  } else {
    stacked_layout_->setCurrentWidget(network_frame_);
  }
}

void FirstBootSetupWindow::onNetworkFinished()
{
    if (GetSettingsBool(kSkipControlPlatformPage)) {
        return onControlPlatformFinished();
    }
    else {
        stacked_layout_->setCurrentWidget(control_platform_frame_);
    }
}

void FirstBootSetupWindow::onControlPlatformFinished()
{
    if (GetSettingsBool(kSkipTimezonePage)) {
        return onTimezoneFinished();
    }
    else {
      stacked_layout_->setCurrentWidget(timezone_frame_);
    }
}

void FirstBootSetupWindow::onTimezoneFinished() {
  // Display loading frame.
  stacked_layout_->setCurrentWidget(loading_frame_);

  emit hook_worker_->startHook();
}

}  // namespace installer
