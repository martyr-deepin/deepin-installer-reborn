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

#include "ui/widgets/spinner_label.h"

#include <QDebug>
#include <QFile>
#include <QTimer>

namespace installer {

namespace {

const int kPixmapLen = 90;

// Animation duration is 25ms.
const int kAnimationDuration = 25;

}  // namespace

SpinnerLabel::SpinnerLabel(QWidget* parent)
    : QLabel(parent),
      animation_timer_(new QTimer(this)),
      pixmaps_(kPixmapLen),
      pixmap_index_(-1) {
  this->setObjectName("spinner_label");

  this->setFixedSize(22, 22);

  for (int i = 0; i < kPixmapLen; ++i) {
    const QString filepath = QString::asprintf(":/spinner/loading_%03d.png", i);
    Q_ASSERT(QFile::exists(filepath));
    pixmaps_[i] = QPixmap(filepath);
  }

  // Read the first frame.
  pixmap_index_ = 0;
  this->setPixmap(pixmaps_.at(pixmap_index_));

  animation_timer_->setInterval(kAnimationDuration);
  connect(animation_timer_, &QTimer::timeout,
          this, &SpinnerLabel::onAnimationTimerTimeout);
}

bool SpinnerLabel::isActive() const {
  return animation_timer_->isActive();
}

void SpinnerLabel::start() {
  if (!animation_timer_->isActive()) {
    animation_timer_->start();
  }
}

void SpinnerLabel::stop() {
  if (animation_timer_->isActive()) {
    animation_timer_->stop();
  }
}

void SpinnerLabel::onAnimationTimerTimeout() {
  pixmap_index_ = (pixmap_index_ + 1) % kPixmapLen;
  Q_ASSERT(pixmap_index_ >= 0);
  Q_ASSERT(pixmap_index_ < kPixmapLen);
  this->setPixmap(pixmaps_.at(pixmap_index_));
}

}  // namespace installer