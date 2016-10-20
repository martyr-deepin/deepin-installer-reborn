// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/install_progress_slide_frame.h"

#include <QDir>

#include "service/signal_manager.h"

namespace installer {

namespace {

const char kDefaultSlide[] = "default";

// Get absolute path to slide folder, based on |locale|.
// |locale| might be empty or like "zh_CN" or "en_US".
QString GetSlideDir(const QString& locale) {
  QDir dir(SLIDE_DIR);
  for (const QFileInfo& fileinfo :
       dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    if (fileinfo.fileName() == locale) {
      return fileinfo.absoluteFilePath();
    }
  }

  return dir.absoluteFilePath(kDefaultSlide);
}

}  // namespace

InstallProgressSlideFrame::InstallProgressSlideFrame(QWidget* parent) :
    QFrame(parent),
    locale_() {
  this->setObjectName(QStringLiteral("install_progress_frame"));

  this->initUI();
  this->initConnections();
}

void InstallProgressSlideFrame::startSlide() {
  const QString slide_dir = GetSlideDir(locale_);
  QDir dir(slide_dir);
  Q_ASSERT(dir.exists());
}

void InstallProgressSlideFrame::stopSlide() {
}

void InstallProgressSlideFrame::initConnections() {
  connect(SignalManager::instance(), &SignalManager::languageSelected,
          this, &InstallProgressSlideFrame::onLanguageUpdated);
}

void InstallProgressSlideFrame::initUI() {

}

void InstallProgressSlideFrame::onLanguageUpdated(const QString& locale) {
  locale_ = locale;
}

}  // namespace installer