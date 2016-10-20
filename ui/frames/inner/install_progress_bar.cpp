// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/install_progress_bar.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

namespace installer {

namespace {

const int kProgressMinimumValue = 0;
const int kProgressMaximumValue = 100;

}

InstallProgressBar::InstallProgressBar(QWidget* parent)
    : QFrame(parent),
      progress_(0) {
  this->setObjectName(QStringLiteral("install_progress_bar"));

  this->setFixedSize(450, 12);
}

void InstallProgressBar::setProgress(int progress) {
  qDebug() << "setProgress():" << progress;
  if (progress < kProgressMinimumValue || progress > kProgressMaximumValue) {
    qWarning() << "setProgress() not in range!" << progress;
    return;
  }
  progress_ = progress;
  this->update();
}

void InstallProgressBar::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);

  const QColor base_color(220, 220, 220, 100);
  painter.fillRect(0, 0, this->width(), this->height(), base_color);

  const QColor fill_color(44, 167, 248, 255);
  painter.fillRect(0, 0, progress_, this->height(), fill_color);
}

}  // namespace installer