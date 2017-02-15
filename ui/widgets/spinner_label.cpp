// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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