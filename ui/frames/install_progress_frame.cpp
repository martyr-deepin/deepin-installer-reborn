// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_progress_frame.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QThread>

#include "service/hooks_manager.h"
#include "ui/frames/consts.h"
#include "ui/frames/inner/install_progress_bar.h"
#include "ui/frames/inner/install_progress_slide_frame.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/title_label.h"

namespace installer {

InstallProgressFrame::InstallProgressFrame(QWidget* parent)
    : QFrame(parent),
      failed_(true),
      hooks_manager_(new HooksManager()),
      hooks_manager_thread_(new QThread()) {
  this->setObjectName(QStringLiteral("install_progress_frame"));

  hooks_manager_->moveToThread(hooks_manager_thread_);
  hooks_manager_thread_->start();

  this->initUI();
  this->initConnections();
}

InstallProgressFrame::~InstallProgressFrame() {
  delete hooks_manager_;
  hooks_manager_ = nullptr;

  hooks_manager_thread_->quit();
  hooks_manager_thread_->wait();
  delete hooks_manager_thread_;
  hooks_manager_thread_ = nullptr;
}

void InstallProgressFrame::startSlide() {
  slide_frame_->startSlide();
}

void InstallProgressFrame::runHooks(bool ok) {
  qDebug() << "runHooks()" << ok;

  if (ok) {
    // Partition operations take 5% progress.
    progress_bar_->setProgress(kBeforeChrootStartVal);

    qDebug() << "emit runHooks() signal";
    // Run hooks/ in background thread.
    emit hooks_manager_->runHooks();
  } else {
    this->onErrorOccurred();
  }
}

void InstallProgressFrame::updateLanguage(const QString& locale) {
  slide_frame_->setLocale(locale);
}

void InstallProgressFrame::initConnections() {
  connect(hooks_manager_, &HooksManager::errorOccurred,
          this, &InstallProgressFrame::onErrorOccurred);
  connect(hooks_manager_, &HooksManager::processUpdate,
          progress_bar_, &InstallProgressBar::setProgress);
  connect(hooks_manager_, &HooksManager::finished,
          this, &InstallProgressFrame::onSucceeded);
}

void InstallProgressFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Installing"));
  CommentLabel* comment_label = new CommentLabel(
      tr("You will be experiencing the incredible pleasant of deepin after "
         "the time for just a cup of coffee"));
  slide_frame_ = new InstallProgressSlideFrame();
  progress_bar_ = new InstallProgressBar();

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addWidget(comment_label, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(slide_frame_, 0, Qt::AlignCenter);
  layout->addWidget(progress_bar_, 0, Qt::AlignCenter);
  layout->addStretch();

  this->setLayout(layout);
}

void InstallProgressFrame::onErrorOccurred() {
  failed_ = true;
  slide_frame_->stopSlide();
  emit this->finished();
}

void InstallProgressFrame::onSucceeded() {
  failed_ = false;
  slide_frame_->stopSlide();
  emit this->finished();
}

}  // namespace installer