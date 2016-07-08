// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/main_window.h"

#include <QGraphicsBlurEffect>
#include <QLabel>
#include <QPainter>
#include "application.h"
#include "service/settings_manager.h"

namespace ui {

MainWindow::MainWindow() : QWidget() {
  this->setObjectName(QStringLiteral("main_window"));

  this->initUI();
  this->registerShortcut();
}

void MainWindow::initUI() {
  background_label_ = new QLabel(this);
  this->updateBackground();

  this->setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::registerShortcut() {

}

void MainWindow::updateBackground() {
  QPixmap background_pixmap;
  background_pixmap.load(app->settings_manager->getWindowBackground());
  background_label_->setPixmap(background_pixmap);
  background_label_->setGraphicsEffect(new QGraphicsBlurEffect());
  QPixmap tmp_background_pixmap(background_pixmap.width(),
                                background_pixmap.height());
  QPainter painter(&tmp_background_pixmap);
  background_label_->render(&painter);
}

}  // namespace ui