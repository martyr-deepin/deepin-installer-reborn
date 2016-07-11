// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/main_window.h"

#include <QGraphicsBlurEffect>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include "application.h"
#include "service/settings_manager.h"
#include "ui/widgets/flat_button.h"

namespace ui {

MainWindow::MainWindow() : QWidget() {
  this->setObjectName(QStringLiteral("main_window"));

  this->initUI();
  this->registerShortcut();
  this->initConnections();
}

void MainWindow::setCloseButtonVisible(bool visible) {
  close_button_->setVisible(visible);
}

bool MainWindow::isCloseButtonVisible() const {
  return close_button_->isVisible();
}

void MainWindow::initConnections() {
}

void MainWindow::initUI() {
  background_label_ = new QLabel(this);
  this->updateBackground();

  close_button_ = new FlatButton(this);
  // TODO(xushaohua): Change close_button_ position in resizeEvent().
  // Move close button to top-right corner of main window.
  close_button_->move(this->width() - 10, 10);
  close_button_->setIcon(QIcon(QStringLiteral(":/images/close_normal.png")));
  close_button_->setFixedSize(32, 32);

  this->setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::registerShortcut() {
  // TODO(xushaohua): Register Ctrl+L and Ctrl+P actions
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