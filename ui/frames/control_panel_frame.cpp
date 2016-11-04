// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/control_panel_frame.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/log_manager.h"

namespace installer {

namespace {

// Read log file each 1000ms.
const int kReadLogInterval = 1000;

}  // namespace

ControlPanelFrame::ControlPanelFrame(QWidget* parent)
    : QFrame(parent),
      timer_(new QTimer(this)),
      log_file_path_(GetLogFilepath()),
      log_content_(){
  this->setObjectName("control_panel_frame");

  timer_->setInterval(kReadLogInterval);
  this->initUI();
  this->initConnections();
}

void ControlPanelFrame::toggleVisible() {
  if (!this->isVisible()) {
    // Slide in
    const QRect geom = this->parentWidget()->geometry();
    const int x_orig = geom.x() - this->width();
    const int x = geom.x();
    const int y_orig = geom.y();
    const int y = geom.y();
    this->move(x_orig, y_orig);
    this->show();
    this->raise();
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos", this);
    animation->setStartValue(QPoint(x_orig, y_orig));
    animation->setEndValue(QPoint(x, y));
    animation->setDuration(300);
    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
    animation->start();

    // Read log file immediately.
    this->onTimerTimeout();
    timer_->start();
  } else {
    // Slide out
    const int x_orig = this->x();
    const int x = x_orig - this->width();
    const int y_orig = this->y();
    const int y = y_orig;
    this->move(x_orig, y_orig);
    this->show();
    this->raise();
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos", this);
    animation->setStartValue(QPoint(x_orig, y_orig));
    animation->setEndValue(QPoint(x, y));
    animation->setDuration(100);
    connect(animation, &QPropertyAnimation::finished,
            this, &ControlPanelFrame::hide);
    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
    animation->start();

    timer_->stop();
  }
}

void ControlPanelFrame::initConnections() {
  connect(timer_, &QTimer::timeout,
          this, &ControlPanelFrame::onTimerTimeout);
}

void ControlPanelFrame::initUI() {
  log_viewer_ = new QTextEdit();
  log_viewer_->setObjectName("log_viewer");
  // Disable user modification.
  log_viewer_->setReadOnly(true);
  // Disable rich text support.
  log_viewer_->setAcceptRichText(false);
  // Disable context menu.
  log_viewer_->setContextMenuPolicy(Qt::NoContextMenu);
  log_viewer_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  log_viewer_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(log_viewer_);
  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/control_panel_frame.css"));
  this->setFixedSize(860, 480);
}

void ControlPanelFrame::onTimerTimeout() {
  const QString content(ReadFile(log_file_path_));
  if (content == log_content_) {
    return;
  }
  log_content_ = content;
  log_viewer_->setPlainText(log_content_);
}

}  // namespace installer