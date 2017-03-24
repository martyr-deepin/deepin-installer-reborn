// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/tests/animations/animations_main_window.h"

#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QObjectList>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/utils/widget_util.h"

namespace installer {

namespace {

const int kWindowWidth = 768;

const int kAnimationDuration = 300;

const int kErrLabelHeight = 35;

const char kErrMsg1[] = "No root partition specified";
const char kErrMsg2[] = "EFI too small";
const char kErrMsg3[] = "No boot specified";

}  // namespace

AnimationsMainWindow::AnimationsMainWindow(QWidget* parent)
    : QFrame(parent),
      msg_labels_() {
  this->setObjectName("main_window");
  this->initUI();
  this->initConnections();
}

void AnimationsMainWindow::initConnections() {
  connect(hide_all_btn_, &QPushButton::clicked,
          this, &AnimationsMainWindow::onHideAllBtnClicked);
  connect(hide_one_btn_, &QPushButton::clicked,
          this, &AnimationsMainWindow::onHideOneBtnClicked);
  connect(show_all_btn_, &QPushButton::clicked,
          this, &AnimationsMainWindow::onShowAllBtnClicked);
  connect(show_one_btn_, &QPushButton::clicked,
          this, &AnimationsMainWindow::onShowOneBtnClicked);
}

void AnimationsMainWindow::initUI() {
  show_all_btn_ = new QPushButton("Show All", this);
  show_one_btn_ = new QPushButton("Show One", this);
  hide_all_btn_ = new QPushButton("Hide All", this);
  hide_one_btn_ = new QPushButton("Hide One", this);

  QHBoxLayout* ctl_layout = new QHBoxLayout();
  ctl_layout->addWidget(show_all_btn_);
  ctl_layout->addWidget(show_one_btn_);
  ctl_layout->addWidget(hide_all_btn_);
  ctl_layout->addWidget(hide_one_btn_);

  msg_layout_ = new QVBoxLayout();
  msg_layout_->setContentsMargins(0, 0, 0, 0);
  msg_layout_->setSpacing(0);

  msg_container_frame_ = new QFrame();
  msg_container_frame_->setContentsMargins(0, 0, 0, 0);
  msg_container_frame_->setObjectName("msg_container_frame");
  msg_container_frame_->setFixedWidth(kWindowWidth);
  msg_container_frame_->setLayout(msg_layout_);
  msg_container_frame_->hide();

  partitions_frame_ = new QFrame();
  partitions_frame_->setObjectName("partitions_frame");
  partitions_frame_->setFixedWidth(kWindowWidth);
  partitions_frame_->setMinimumHeight(500);

  QVBoxLayout* scroll_layout = new QVBoxLayout();
  scroll_layout->addWidget(msg_container_frame_);
  scroll_layout->addWidget(partitions_frame_);

  scroll_frame_ = new QFrame();
  scroll_frame_->setObjectName("scroll_frame");
  scroll_frame_->setLayout(scroll_layout);

  scroll_area_ = new QScrollArea();
  scroll_area_->setWidget(scroll_frame_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addLayout(ctl_layout);
  layout->addWidget(scroll_area_);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/main_window.css"));
//  this->setAttribute(Qt::WA_TranslucentBackground, true);
  this->setFixedSize(kWindowWidth, 768);
}


void AnimationsMainWindow::showMessage(QLabel* label) {
  Q_UNUSED(label);
}

void AnimationsMainWindow::hideMessage(QLabel* label) {
  Q_UNUSED(label);
}

void AnimationsMainWindow::onHideAllBtnClicked() {
  QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
  QPropertyAnimation* height_animation = new QPropertyAnimation(this);
  height_animation->setTargetObject(msg_container_frame_);
  height_animation->setPropertyName("maximumHeight");
//  height_animation->setStartValue(200);
  height_animation->setEndValue(0);
  height_animation->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation->setDuration(kAnimationDuration);
  group->addAnimation(height_animation);
  group->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationsMainWindow::onHideOneBtnClicked() {
  if (msg_labels_.isEmpty()) {
    return;
  }

  QLabel* label = msg_labels_.first();
  msg_labels_.removeFirst();

  QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
  QPropertyAnimation* height_animation = new QPropertyAnimation(this);
  height_animation->setTargetObject(label);
  height_animation->setPropertyName("maximumHeight");
  height_animation->setEndValue(0);
  height_animation->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation->setDuration(kAnimationDuration);
  group->addAnimation(height_animation);
  connect(group, &QAnimationGroup::finished,
          label, &QLabel::deleteLater);
  group->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationsMainWindow::onShowAllBtnClicked() {
  ClearLayout(msg_layout_);

  msg_container_frame_->show();

  QLabel* label1 = new QLabel(kErrMsg1, msg_container_frame_);
  label1->show();
  msg_labels_.append(label1);
  msg_layout_->addWidget(label1);
  QLabel* label2 = new QLabel(kErrMsg2, msg_container_frame_);
  label2->show();
  msg_labels_.append(label2);
  msg_layout_->addWidget(label2);
  QLabel* label3 = new QLabel(kErrMsg3, msg_container_frame_);
  label3->show();
  msg_labels_.append(label3);
  msg_layout_->addWidget(label3);

  QParallelAnimationGroup* group = new QParallelAnimationGroup(this);

  const int height = label1->height() + label2->height() + label3->height();
  QPropertyAnimation* height_animation = new QPropertyAnimation(this);
  height_animation->setTargetObject(msg_container_frame_);
  height_animation->setPropertyName("geometry");
  QRect fuck = msg_container_frame_->geometry();
  fuck.setHeight(fuck.height() + height);
  height_animation->setEndValue(fuck);
  height_animation->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation->setDuration(kAnimationDuration);
  group->addAnimation(height_animation);
  group->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationsMainWindow::onShowOneBtnClicked() {
  QLabel* label = new QLabel(kErrMsg1, msg_container_frame_);
  msg_labels_.append(label);
  msg_layout_->addWidget(label);
  label->show();

  QPropertyAnimation* height_animation2 = new QPropertyAnimation(this);
  height_animation2->setTargetObject(msg_container_frame_);
  height_animation2->setPropertyName("geometry");
  QRect fuck = msg_container_frame_->geometry();
  fuck.setHeight(fuck.height() + label->height());
  height_animation2->setStartValue(fuck);
  height_animation2->setEndValue(fuck);
  height_animation2->setEasingCurve(QEasingCurve::InOutCubic);
  height_animation2->setDuration(kAnimationDuration);

  QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
  group->addAnimation(height_animation2);
  group->start(QAbstractAnimation::DeleteWhenStopped);
}

}  // namespace installer