// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/install_progress_slide_frame.h"

#include <QDebug>
#include <QDir>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

namespace installer {

namespace {

const char kDefaultSlide[] = "default";

// Duration of each slide.
const int kSlideDuration = 5000;

// Get absolute path to slide folder, based on |locale|.
// |locale| might be empty or like "zh_CN" or "en_US".
QString GetSlideDir(const QString& locale) {
  QDir dir(RESOURCES_DIR "/slide");
  Q_ASSERT(dir.exists());
  for (const QFileInfo& fileinfo :
       dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    if (fileinfo.fileName() == locale) {
      return fileinfo.absoluteFilePath();
    }
  }

  return dir.absoluteFilePath(kDefaultSlide);
}

}  // namespace

InstallProgressSlideFrame::InstallProgressSlideFrame(QWidget* parent)
    : QFrame(parent),
      locale_(),
      slide_index_(0),
      slide_files_() {
  this->setObjectName("install_progress_frame");

  this->initUI();
  this->initConnections();
}


void InstallProgressSlideFrame::setLocale(const QString& locale) {
  locale_ = locale;
}

void InstallProgressSlideFrame::startSlide(bool position_animation,
                                           bool opacity_animation) {
  QDir slide_dir(GetSlideDir(locale_));
  Q_ASSERT(slide_dir.exists());
  for (const QString& filename : slide_dir.entryList(QDir::Files)) {
    slide_files_.append(slide_dir.absoluteFilePath(filename));
  }
  if (slide_files_.isEmpty()) {
    qCritical() << "startSlide() no slide files found!" << slide_dir;
    return;
  }
  slide_index_ = 0;
  this->updateSlideImage();

  animation_group_->clear();
  if (position_animation) {
    animation_group_->addAnimation(pos_animation_);
  }
  if (opacity_animation) {
    animation_group_->addAnimation(opacity_animation_);
  }
  animation_group_->start();
}

void InstallProgressSlideFrame::stopSlide() {
  animation_group_->stop();
}

void InstallProgressSlideFrame::initConnections() {
  connect(animation_group_, &QParallelAnimationGroup::currentLoopChanged,
          this, &InstallProgressSlideFrame::onAnimationCurrentLoopChanged);
}

void InstallProgressSlideFrame::initUI() {
  container_label_ = new QLabel(this);

  pos_animation_ = new QPropertyAnimation(container_label_, "pos", this);
  pos_animation_->setKeyValueAt(0.0, QPoint(-50, 0));
  pos_animation_->setKeyValueAt(0.1, QPoint(0, 0));
  pos_animation_->setKeyValueAt(0.9, QPoint(0, 0));
  pos_animation_->setKeyValueAt(1.0, QPoint(50, 0));
  pos_animation_->setDuration(kSlideDuration);

  opacity_effect_ = new QGraphicsOpacityEffect(container_label_);
  container_label_->setGraphicsEffect(opacity_effect_);
  opacity_animation_ = new QPropertyAnimation(opacity_effect_, "opacity", this);
  opacity_animation_->setKeyValueAt(0.0, 0.0);
  opacity_animation_->setKeyValueAt(0.1, 1.0);
  opacity_animation_->setKeyValueAt(0.9, 1.0);
  opacity_animation_->setKeyValueAt(1.0, 0.0);
  opacity_animation_->setDuration(kSlideDuration);

  animation_group_ = new QParallelAnimationGroup(this);
  animation_group_->setLoopCount(-1);
  // Slide window size shall be the same with slide images
  this->setFixedSize(960, 453);
}

void InstallProgressSlideFrame::updateSlideImage() {
  Q_ASSERT(slide_index_ < slide_files_.length());
  const QPixmap pixmap(slide_files_.at(slide_index_));
  container_label_->setPixmap(pixmap);
  container_label_->show();
  slide_index_ = (slide_index_ + 1) % slide_files_.length();
}

void InstallProgressSlideFrame::onAnimationCurrentLoopChanged() {
  this->updateSlideImage();
}

}  // namespace installer