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

void AnimationShowWidget(QWidget* widget) {
  qDebug() << "showWidget():" << widget;
  Q_ASSERT(widget != nullptr);

  // widget takes ownership of opacity_effect.
  QGraphicsOpacityEffect* opacity_effect = new QGraphicsOpacityEffect(nullptr);
  widget->setGraphicsEffect(opacity_effect);
  QPropertyAnimation* opacity_animation = new QPropertyAnimation(nullptr);
  opacity_animation->setTargetObject(opacity_effect);
  opacity_animation->setPropertyName("opacity");
  opacity_animation->setStartValue(0.05);
  opacity_animation->setEndValue(0.95);
  opacity_animation->setEasingCurve(QEasingCurve::InOutCubic);
  opacity_animation->setDuration(kAnimationInterval);

  QPropertyAnimation* height_animation = new QPropertyAnimation(nullptr);
  height_animation->setTargetObject(widget);
  // Only change maxHeight property of |widget|.
  height_animation->setPropertyName("maximumHeight");
  const int current_height = widget->height();
  height_animation->setStartValue(0);
  height_animation->setEndValue(current_height);
  height_animation->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation->setDuration(kAnimationInterval);

  QParallelAnimationGroup* animation_group = new QParallelAnimationGroup(nullptr);
  animation_group->addAnimation(opacity_animation);
  animation_group->addAnimation(height_animation);
  animation_group->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationHideWidget(QWidget* widget) {
  qDebug() << "hideWidget:" << widget;
  Q_ASSERT(widget != nullptr);

  // widget takes ownership of opacity_effect.
  QGraphicsOpacityEffect* opacity_effect = new QGraphicsOpacityEffect(nullptr);
  widget->setGraphicsEffect(opacity_effect);

  QParallelAnimationGroup* animation_group =
      new QParallelAnimationGroup(nullptr);
  QPropertyAnimation* opacity_animation = new QPropertyAnimation(nullptr);
  QPropertyAnimation* height_animation = new QPropertyAnimation(nullptr);
  animation_group->addAnimation(opacity_animation);
  animation_group->addAnimation(height_animation);

  opacity_animation->setTargetObject(opacity_effect);
  opacity_animation->setPropertyName("opacity");
  opacity_animation->setStartValue(0.95);
  opacity_animation->setEndValue(0.05);
  opacity_animation->setEasingCurve(QEasingCurve::InOutCubic);
  opacity_animation->setDuration(kAnimationInterval);

  height_animation->setTargetObject(widget);
  // Only change maxHeight property of |widget|.
  height_animation->setPropertyName("maximumHeight");
  const int current_height = widget->height();
  height_animation->setStartValue(current_height);
  height_animation->setEndValue(0);
  height_animation->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation->setDuration(kAnimationInterval);

  QObject::connect(animation_group, &QAnimationGroup::finished, [=]() {
    widget->setMaximumWidth(current_height);
  });
  animation_group->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationHighlightPartitionButton(AdvancedPartitionButton* button) {
  Q_ASSERT(button != nullptr);

  QPropertyAnimation* alpha_animation = new QPropertyAnimation(nullptr);
  alpha_animation->setPropertyName("alpha");
  alpha_animation->setTargetObject(button);
  alpha_animation->setStartValue(0.0);
  alpha_animation->setEndValue(0.3);
  alpha_animation->setEasingCurve(QEasingCurve::InOutCubic);
  alpha_animation->setDuration(kButtonAnimationInterval);
  alpha_animation->start(QAbstractAnimation::DeleteWhenStopped);
}

}  // namespace installer
