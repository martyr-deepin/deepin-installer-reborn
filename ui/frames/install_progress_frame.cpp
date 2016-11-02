// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_progress_frame.h"

#include <QDebug>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QThread>

#include "base/file_util.h"
#include "service/hooks_manager.h"
#include "ui/frames/consts.h"
#include "ui/frames/inner/install_progress_slide_frame.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/install_progress_tip.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const int kProgressBarWidth = 640;
const int kTooltipWidth = 60;
const int kTooltipHeight = 31;
const int kTooltipFrameWidth = kProgressBarWidth + kTooltipWidth;

}  // namespace

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
  this->onProgressUpdate(47);
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
    this->onProgressUpdate(kBeforeChrootStartVal);

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
          this, &InstallProgressFrame::onProgressUpdate);
  connect(hooks_manager_, &HooksManager::finished,
          this, &InstallProgressFrame::onSucceeded);
}

void InstallProgressFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Installing"));
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("You will be experiencing the incredible pleasant of deepin after "
         "the time for just a cup of coffee"));
  slide_frame_ = new InstallProgressSlideFrame();

  QFrame* tooltip_frame = new QFrame();
  tooltip_frame->setObjectName("tooltip_frame");
  tooltip_frame->setContentsMargins(0, 0, 0, 0);
  tooltip_frame->setFixedSize(kTooltipFrameWidth, kTooltipHeight);
  tooltip_label_ = new InstallProgressTip(tooltip_frame);
  tooltip_label_->setFixedSize(kTooltipWidth, kTooltipHeight);
  tooltip_label_->setAlignment(Qt::AlignHCenter);

  progress_bar_ = new QProgressBar();
  progress_bar_->setFixedSize(kProgressBarWidth, 8);
  progress_bar_->setTextVisible(false);
  progress_bar_->setRange(0, 100);
  progress_bar_->setOrientation(Qt::Horizontal);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(slide_frame_, 0, Qt::AlignCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addWidget(tooltip_frame, 0, Qt::AlignHCenter);
  layout->addSpacing(5);
  layout->addWidget(progress_bar_, 0, Qt::AlignCenter);
  layout->addStretch();

  this->setLayout(layout);
  this->setStyleSheet(
      ReadTextFileContent(":/styles/install_progress_frame.css"));
}

void InstallProgressFrame::onErrorOccurred() {
  failed_ = true;
  slide_frame_->stopSlide();
  emit this->finished();
}

void InstallProgressFrame::onProgressUpdate(int progress) {
  progress_bar_->setValue(progress);
  tooltip_label_->setText(QString("%1%").arg(progress));
  const int x = kProgressBarWidth * progress / 100;
  const int y = tooltip_label_->y();
  tooltip_label_->move(x, y);
  // Force QProgressBar to repaint.
  progress_bar_->update();
}

void InstallProgressFrame::onSucceeded() {
  failed_ = false;
  slide_frame_->stopSlide();
  emit this->finished();
}

}  // namespace installer