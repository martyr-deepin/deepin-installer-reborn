/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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

#include "ui/user_form_window.h"

#include <QDebug>
#include <QLabel>
#include <QStackedLayout>

#include "service/settings_manager.h"
#include "ui/frames/system_info_frame.h"
//#include "ui/frames/timezone_frame.h"
#include "ui/utils/widget_util.h"

namespace installer {

UserFormWindow::UserFormWindow(QWidget* parent) : QWidget(parent) {
  this->initUI();
  this->initPages();
  this->initConnections();
  system_info_frame_->readConf();
}

UserFormWindow::~UserFormWindow() {

}

void UserFormWindow::fullscreen() {
  ShowFullscreen(this);
}

void UserFormWindow::initConnections() {
  connect(system_info_frame_, &SystemInfoFrame::finished,
          this, &UserFormWindow::onUserFormFinished);
}

void UserFormWindow::initPages() {
  system_info_frame_ = new SystemInfoFrame(this);
  stacked_layout_->addWidget(system_info_frame_);
}

void UserFormWindow::initUI() {
  background_label_ = new QLabel(this);

  stacked_layout_ = new QStackedLayout();

  QVBoxLayout* vbox_layout = new QVBoxLayout();
  vbox_layout->setContentsMargins(0, 10, 0, 0);
  vbox_layout->setSpacing(0);
  vbox_layout->addLayout(stacked_layout_);
  vbox_layout->addSpacing(32);

  this->setLayout(vbox_layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setWindowFlags(Qt::FramelessWindowHint);
}

void UserFormWindow::resizeEvent(QResizeEvent* event) {
  this->updateBackground();
  QWidget::resizeEvent(event);
}

void UserFormWindow::updateBackground() {
  if (!background_label_) {
    qWarning() << "background_label is not initialized!";
    return;
  }
  const QString image_path = GetWindowBackground();
  // Other platforms may have performance issue.
  const QPixmap pixmap =
      QPixmap(image_path).scaled(size(), Qt::KeepAspectRatioByExpanding);
  background_label_->setPixmap(pixmap);
  background_label_->setFixedSize(size());
}

void UserFormWindow::onUserFormFinished() {
  this->close();
}

}  // namespace installer