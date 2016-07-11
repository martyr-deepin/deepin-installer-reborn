// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/main_window.h"

#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>
#include <QShowEvent>
#include "application.h"
#include "service/settings_manager.h"
#include "ui/widgets/icon_button.h"

namespace ui {

namespace {

const int kCloseButtonSize = 32;

}

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

void MainWindow::resizeEvent(QResizeEvent* event) {
  Q_ASSERT(background_label_);
  Q_ASSERT(close_button_);

  this->updateBackground();

  // Move close button to top-right corner of main window.
  // Do not call event->size().width(), which stores an incorrect value.
  close_button_->move(this->width() - kCloseButtonSize - 10, 10);

  QWidget::resizeEvent(event);
}

void MainWindow::initConnections() {
}

void MainWindow::initUI() {
  background_label_ = new QLabel(this);

  close_button_ = new IconButton(QStringLiteral(":/images/close_normal.png"),
                                 QStringLiteral(":/images/close_hover.png"),
                                 QStringLiteral(":/images/close_press.png"),
                                 kCloseButtonSize,
                                 kCloseButtonSize,
                                 this);

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