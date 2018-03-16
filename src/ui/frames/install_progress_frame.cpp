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

#include "ui/frames/install_progress_frame.h"

#include <math.h>
#include <QDebug>
#include <QEvent>
#include <QPropertyAnimation>
#include <QStyle>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "base/thread_util.h"
#include "service/hooks_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/frames/inner/install_progress_slide_frame.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/rounded_progress_bar.h"
#include "ui/widgets/tooltip_pin.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const int kProgressBarWidth = 640;
const int kTooltipWidth = 60;
const int kTooltipHeight = 31;
const int kTooltipLabelMargin = 2;
const int kTooltipFrameWidth = kProgressBarWidth + kTooltipWidth;

const int kRetainingInterval = 3000;

const int kSimulationTimerInterval = 3000;

const int kProgressAnimationDuration = 500;

}  // namespace

InstallProgressFrame::InstallProgressFrame(QWidget* parent)
    : QFrame(parent),
      failed_(true),
      progress_(0),
      hooks_manager_(new HooksManager()),
      hooks_manager_thread_(new QThread(this)),
      simulation_timer_(new QTimer(this)) {
  this->setObjectName("install_progress_frame");

  hooks_manager_->moveToThread(hooks_manager_thread_);

  this->initUI();
  this->initConnections();

  hooks_manager_thread_->start();

  simulation_timer_->setSingleShot(false);
  simulation_timer_->setInterval(kSimulationTimerInterval);
}

InstallProgressFrame::~InstallProgressFrame() {
  QuitThread(hooks_manager_thread_);
}

void InstallProgressFrame::startSlide() {
  const bool disable_slide =
      GetSettingsBool(kInstallProgressPageDisableSlide);
  const bool disable_animation =
      GetSettingsBool(kInstallProgressPageDisableSlideAnimation);
  const int duration = GetSettingsInt(kInstallProgressPageAnimationDuration);
  slide_frame_->startSlide(disable_slide, disable_animation, duration);
}

void InstallProgressFrame::simulate() {
  if (!simulation_timer_->isActive()) {
    this->startSlide();

    // Reset progress value.
    this->onProgressUpdate(progress_bar_->minimum());
    simulation_timer_->start();
  }
}

void InstallProgressFrame::runHooks(bool ok) {
  qDebug() << "runHooks()" << ok;

  if (ok) {
    // Partition operations take 5% progress.
    this->onProgressUpdate(kBeforeChrootStartVal);

    qDebug() << "emit runHooks() signal";
    // Notify HooksManager to run hooks/ in background thread.
    // Do not run hooks in debug mode.
#ifdef NDEBUG
    emit hooks_manager_->runHooks();
#endif
  } else {
    this->onHooksErrorOccurred();
  }
}

void InstallProgressFrame::setProgress(int progress) {
  progress_ = progress;
  this->updateProgressBar(progress);
}

void InstallProgressFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Installing"));
    comment_label_->setText(
        tr("You can experience the incredible pleasure of deepin after "
           "the time for just a cup of coffee"));
  } else {
    QFrame::changeEvent(event);
  }
}

void InstallProgressFrame::initConnections() {
  connect(hooks_manager_, &HooksManager::errorOccurred,
          this, &InstallProgressFrame::onHooksErrorOccurred);
  connect(hooks_manager_, &HooksManager::finished,
          this, &InstallProgressFrame::onHooksFinished);
  connect(hooks_manager_, &HooksManager::processUpdate,
          this, &InstallProgressFrame::onProgressUpdate);

  connect(hooks_manager_thread_, &QThread::finished,
          hooks_manager_, &HooksManager::deleteLater);

  connect(simulation_timer_, &QTimer::timeout,
          this, &InstallProgressFrame::onSimulationTimerTimeout);
}

void InstallProgressFrame::initUI() {
  title_label_ = new TitleLabel(tr("Installing"));
  comment_label_ = new CommentLabel(
      tr("You can experience the incredible pleasure of deepin after "
         "the time for just a cup of coffee"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  slide_frame_ = new InstallProgressSlideFrame();

  QFrame* tooltip_frame = new QFrame();
  tooltip_frame->setObjectName("tooltip_frame");
  tooltip_frame->setContentsMargins(0, 0, 0, 0);
  tooltip_frame->setFixedSize(kTooltipFrameWidth, kTooltipHeight);
  tooltip_label_ = new TooltipPin(tooltip_frame);
  tooltip_label_->setFixedSize(kTooltipWidth, kTooltipHeight);
  tooltip_label_->setAlignment(Qt::AlignHCenter);
  tooltip_label_->setText("0%");
  // Add left margin.
  tooltip_label_->move(kTooltipLabelMargin, tooltip_label_->y());

  // NOTE(xushaohua): QProgressBar::paintEvent() has performance issue on
  // loongson platform, when chunk style is set. So we override paintEvent()
  // and draw progress bar chunk by hand.
  progress_bar_ = new RoundedProgressBar();
  progress_bar_->setObjectName("progress_bar");
  progress_bar_->setFixedSize(kProgressBarWidth, 8);
  progress_bar_->setTextVisible(false);
  // Set progress range to [0, 1000] so that progress bar can be painted
  // more smoothly.
  progress_bar_->setRange(0, 1000);
  progress_bar_->setOrientation(Qt::Horizontal);
  progress_bar_->setValue(0);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(slide_frame_, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(tooltip_frame, 0, Qt::AlignHCenter);
  layout->addSpacing(5);
  layout->addWidget(progress_bar_, 0, Qt::AlignCenter);
  layout->addStretch();

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setStyleSheet(ReadFile(":/styles/install_progress_frame.css"));

  progress_animation_ = new QPropertyAnimation(this, "progress", this);
  progress_animation_->setDuration(kProgressAnimationDuration);
  progress_animation_->setEasingCurve(QEasingCurve::InOutCubic);
}

void InstallProgressFrame::updateProgressBar(int progress) {
  Q_ASSERT(progress_bar_->maximum() != 0);
  progress_bar_->setValue(progress);
  const double percentage = progress * 1.0 / progress_bar_->maximum();

  // Calculate percentage of progress.
  const int real_progress = int(floor(percentage * 100.0));
  tooltip_label_->setText(QString("%1%").arg(real_progress));
  // Add right margin.
  int x = int(kProgressBarWidth * percentage - kTooltipLabelMargin);
  if (x < kTooltipLabelMargin) {
    // Add left margin.
    x = kTooltipLabelMargin;
  }
  const int y = tooltip_label_->y();
  tooltip_label_->move(x, y);

  // Force QProgressBar to repaint.
  this->style()->unpolish(progress_bar_);
  this->style()->polish(progress_bar_);
  progress_bar_->repaint();
}

void InstallProgressFrame::onHooksErrorOccurred() {
  failed_ = true;
  slide_frame_->stopSlide();
  emit this->finished();
}

void InstallProgressFrame::onHooksFinished() {
  failed_ = false;

  // Set progress value to 100 explicitly.
  this->onProgressUpdate(100);

  QTimer::singleShot(kRetainingInterval,
                     this, &InstallProgressFrame::onRetainingTimerTimeout);
}

void InstallProgressFrame::onProgressUpdate(int progress) {
  // Multiple progress value by 10 to fit progress_bar_ range.
  const int virtual_progress = progress * 10;
  progress_animation_->setEndValue(virtual_progress);
  progress_animation_->start();
}

void InstallProgressFrame::onRetainingTimerTimeout() {
  slide_frame_->stopSlide();
  emit this->finished();
}

void InstallProgressFrame::onSimulationTimerTimeout() {
  // Increase 5% each time.
  const int progress = (progress_bar_->value() + 10) * 100
                       / progress_bar_->maximum();
  if (progress > progress_bar_->maximum()) {
    simulation_timer_->stop();
  } else {
    this->onProgressUpdate(progress);
  }
}

}  // namespace installer