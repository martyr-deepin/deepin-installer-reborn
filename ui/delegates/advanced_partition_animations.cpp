// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/advanced_partition_animations.h"

#include <math.h>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QWidget>

#include "ui/widgets/advanced_partition_button.h"

namespace installer {

namespace {

// Animation duration is 300ms.
const int kAnimationInterval = 300;

// Animation duration of partition button is 200ms.
const int kButtonAnimationInterval = 200;

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
  opacity_animation_->setEasingCurve(QEasingCurve::InOutCubic);
  opacity_animation_->setDuration(kAnimationInterval);

  height_animation_->setTargetObject(widget);
  height_animation_->setPropertyName("maximumHeight");
  // Only change maxHeight property of |widget|.
  const int current_height = widget->height();
  const int start_height = int(floor(current_height * 0.8));
  height_animation_->setStartValue(start_height);
  height_animation_->setEndValue(current_height);
  height_animation_->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation_->setDuration(kAnimationInterval);

  animation_group_->clear();
  animation_group_->addAnimation(opacity_animation_);
  animation_group_->addAnimation(height_animation_);
  animation_group_->start();
}

void AdvancedPartitionAnimations::hideWidget(QWidget* widget) {
  Q_ASSERT(widget != nullptr);
}

void AdvancedPartitionAnimations::highlightPartitionButton(
    AdvancedPartitionButton* button) {
  Q_ASSERT(button != nullptr);

  alpha_animation_->setTargetObject(button);
  alpha_animation_->setStartValue(0.0);
  alpha_animation_->setEndValue(0.3);
  alpha_animation_->setEasingCurve(QEasingCurve::InOutCubic);
  alpha_animation_->setDuration(kButtonAnimationInterval);
  alpha_animation_->start();
}

void AdvancedPartitionAnimations::initAnimations() {
  animation_group_ = new QParallelAnimationGroup(this);
  opacity_effect_ = new QGraphicsOpacityEffect(this);
  opacity_animation_ = new QPropertyAnimation(opacity_effect_, "opacity", this);

  height_animation_ = new QPropertyAnimation(this);

  alpha_animation_ = new QPropertyAnimation(this);
  alpha_animation_->setPropertyName("alpha");
}

}  // namespace installer