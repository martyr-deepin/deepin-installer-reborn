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
}

void AdvancedPartitionAnimations::showWidget(QWidget* widget) {
  qDebug() << "showWidget():" << widget;
  Q_ASSERT(widget != nullptr);
  if (widget == nullptr) {
    return;
  }

  // widget takes ownership of opacity_effect.
  QGraphicsOpacityEffect* opacity_effect = new QGraphicsOpacityEffect(this);
  widget->setGraphicsEffect(opacity_effect);

  QPropertyAnimation* opacity_animation = new QPropertyAnimation(this);
  opacity_animation->setTargetObject(opacity_effect);
  opacity_animation->setPropertyName("opacity");
  opacity_animation->setStartValue(0.0);
  opacity_animation->setEndValue(1.0);
  opacity_animation->setEasingCurve(QEasingCurve::InOutCubic);
  opacity_animation->setDuration(kAnimationInterval);

  QPropertyAnimation* height_animation = new QPropertyAnimation(this);
  height_animation->setTargetObject(widget);
  // Only change maxHeight property of |widget|.
  height_animation->setPropertyName("maximumHeight");
  const int current_height = widget->height();
  const int start_height = int(floor(current_height * 0.8));
  height_animation->setStartValue(start_height);
  height_animation->setEndValue(current_height);
  height_animation->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation->setDuration(kAnimationInterval);

  QParallelAnimationGroup* animation_group = new QParallelAnimationGroup(this);
  animation_group->addAnimation(opacity_animation);
  animation_group->addAnimation(height_animation);

  connect(animation_group, &QAnimationGroup::finished, [=]() {
    opacity_animation->deleteLater();
    height_animation->deleteLater();
    animation_group->deleteLater();
  });

  animation_group->start();
}

void AdvancedPartitionAnimations::hideWidget(QWidget* widget) {
  qDebug() << "hideWidget:" << widget;
  Q_ASSERT(widget != nullptr);

  // widget takes ownership of opacity_effect.
  QGraphicsOpacityEffect* opacity_effect = new QGraphicsOpacityEffect(this);
  widget->setGraphicsEffect(opacity_effect);

  QParallelAnimationGroup* animation_group =
      new QParallelAnimationGroup(this);
  QPropertyAnimation* opacity_animation = new QPropertyAnimation(this);
  QPropertyAnimation* height_animation = new QPropertyAnimation(this);
  animation_group->addAnimation(opacity_animation);
  animation_group->addAnimation(height_animation);

  opacity_animation->setTargetObject(opacity_effect);
  opacity_animation->setPropertyName("opacity");
  opacity_animation->setStartValue(1.0);
  opacity_animation->setEndValue(0.0);
  opacity_animation->setEasingCurve(QEasingCurve::InOutCubic);
  opacity_animation->setDuration(kAnimationInterval);

  height_animation->setTargetObject(widget);
  // Only change maxHeight property of |widget|.
  height_animation->setPropertyName("maximumHeight");
  const int current_height = widget->height();
  const int end_height = int(floor(current_height * 0.8));
  height_animation->setStartValue(current_height);
  height_animation->setEndValue(end_height);
  height_animation->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation->setDuration(kAnimationInterval);

  connect(animation_group, &QAnimationGroup::finished, [=]() {
    opacity_animation->deleteLater();
    height_animation->deleteLater();
    animation_group->deleteLater();
    widget->hide();
    widget->setMaximumWidth(current_height);
  });
  animation_group->start();
}

void AdvancedPartitionAnimations::highlightPartitionButton(
    AdvancedPartitionButton* button) {
  Q_ASSERT(button != nullptr);

  QPropertyAnimation* alpha_animation = new QPropertyAnimation(this);
  alpha_animation->setPropertyName("alpha");
  alpha_animation->setTargetObject(button);
  alpha_animation->setStartValue(0.0);
  alpha_animation->setEndValue(0.3);
  alpha_animation->setEasingCurve(QEasingCurve::InOutCubic);
  alpha_animation->setDuration(kButtonAnimationInterval);
  connect(alpha_animation, &QPropertyAnimation::finished,
          alpha_animation, &QPropertyAnimation::deleteLater);
  alpha_animation->start();
}

}  // namespace installer