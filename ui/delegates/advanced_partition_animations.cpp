// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/advanced_partition_animations.h"

#include <math.h>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QWidget>

namespace installer {

namespace {

// Animation duration is 300ms.
const int kAnimationInterval = 300;

}  // namespace

AdvancedPartitionAnimations::AdvancedPartitionAnimations(QObject* parent)
    : QObject(parent) {
  this->setObjectName("advanced_partition_animations");

  this->initAnimations();
}

void AdvancedPartitionAnimations::showWidget(QWidget* widget) {
  Q_ASSERT(widget != nullptr);
  widget->setGraphicsEffect(opacity_effect_);
  opacity_animation_->setStartValue(0.0);
  opacity_animation_->setEndValue(1.0);

  height_animation_->setTargetObject(widget);
  height_animation_->setPropertyName("maximumHeight");
  // Only change maxHeight property of |widget|.
  const int current_height = widget->height();
  const int start_height = int(floor(current_height * 0.8));
  height_animation_->setStartValue(start_height);
  height_animation_->setEndValue(current_height);

  animation_group_->addAnimation(opacity_animation_);
  animation_group_->addAnimation(height_animation_);
  animation_group_->start();
}

void AdvancedPartitionAnimations::hideWidget(QWidget* widget) {
  Q_ASSERT(widget != nullptr);
}

void AdvancedPartitionAnimations::initAnimations() {
  animation_group_ = new QParallelAnimationGroup(this);
  opacity_effect_ = new QGraphicsOpacityEffect(this);
  opacity_animation_ = new QPropertyAnimation(opacity_effect_, "opacity", this);
  opacity_animation_->setEasingCurve(QEasingCurve::InOutCubic);
  opacity_animation_->setDuration(kAnimationInterval);

  height_animation_ = new QPropertyAnimation(this);
  height_animation_->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation_->setDuration(kAnimationInterval);
}

}  // namespace installer