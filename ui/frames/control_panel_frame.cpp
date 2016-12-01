// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/control_panel_frame.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QTabBar>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/log_manager.h"
#include "ui/widgets/table_combo_box.h"

namespace installer {

namespace {

// Read log file each 1000ms.
const int kReadLogInterval = 1000;

const int kWindowWidth = 860;
const int kWindowHeight = 480;

}  // namespace

ControlPanelFrame::ControlPanelFrame(QWidget* parent)
    : QFrame(parent),
      timer_(new QTimer(this)),
      log_file_path_(GetLogFilepath()),
      log_content_(){
  this->setObjectName("control_panel_frame");

  timer_->setInterval(kReadLogInterval);
  this->initUI();
  this->initConnections();
}

void ControlPanelFrame::toggleVisible() {
  if (!this->isVisible()) {
    // Slide in
    const QRect geom = this->parentWidget()->geometry();
    const int x_orig = geom.x() - this->width();
    const int x = geom.x();
    const int y_orig = geom.y();
    const int y = geom.y();
    this->move(x_orig, y_orig);
    this->show();
    this->raise();
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos", this);
    animation->setStartValue(QPoint(x_orig, y_orig));
    animation->setEndValue(QPoint(x, y));
    animation->setDuration(300);
    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
    animation->start();

    // Read log file immediately.
    this->onTimerTimeout();
    timer_->start();
  } else {
    // Slide out
    const int x_orig = this->x();
    const int x = x_orig - this->width();
    const int y_orig = this->y();
    const int y = y_orig;
    this->move(x_orig, y_orig);
    this->show();
    this->raise();
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos", this);
    animation->setStartValue(QPoint(x_orig, y_orig));
    animation->setEndValue(QPoint(x, y));
    animation->setDuration(100);
    connect(animation, &QPropertyAnimation::finished,
            this, &ControlPanelFrame::hide);
    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
    animation->start();

    timer_->stop();
  }
}

void ControlPanelFrame::initConnections() {
  connect(page_combo_box_,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &ControlPanelFrame::currentPageChanged);
  connect(tab_bar_, &QTabBar::currentChanged,
          stacked_widget_, &QStackedWidget::setCurrentIndex);
  connect(timer_, &QTimer::timeout,
          this, &ControlPanelFrame::onTimerTimeout);
}

void ControlPanelFrame::initUI() {
  tab_bar_ = new QTabBar();
  tab_bar_->setShape(QTabBar::RoundedEast);
  tab_bar_->addTab("Log");
  tab_bar_->addTab("Pages");
  tab_bar_->addTab("Settings");

  log_viewer_ = new QTextEdit();
  log_viewer_->setObjectName("log_viewer");
  // Disable user modification.
  log_viewer_->setReadOnly(true);
  // Disable rich text support.
  log_viewer_->setAcceptRichText(false);
  // Disable context menu.
  log_viewer_->setContextMenuPolicy(Qt::NoContextMenu);
  log_viewer_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  log_viewer_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  page_combo_box_ = new TableComboBox();
  page_combo_box_->addItems({"ConfirmQuitFrame",
                             "DiskSpaceInsufficientFrame",
                             "InstallFailedFrame",
                             "InstallProgressFrame",
                             "InstallSuccessFrame",
                             "PartitionFrame",
                             "PartitionTableWarningFrame",
                             "SelectLanguageFrame",
                             "SystemInfoFrame",
                             "VirtualMachineFrame",
                            });

  settings_page_ = new QFrame();

  stacked_widget_ = new QStackedWidget();
  stacked_widget_->addWidget(log_viewer_);
  stacked_widget_->addWidget(page_combo_box_);
  stacked_widget_->addWidget(settings_page_);

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(stacked_widget_);
  layout->addWidget(tab_bar_);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/control_panel_frame.css"));
  this->setFixedSize(kWindowWidth, kWindowHeight);
}

void ControlPanelFrame::onTimerTimeout() {
  const QString content(ReadFile(log_file_path_));
  if (content.length() == log_content_.length()) {
    return;
  }
  log_content_ = content;

  QTextCursor cursor = log_viewer_->textCursor();
  // Restore cursor position when log content is updated.
  const int pos = cursor.position();
  log_viewer_->setPlainText(log_content_);
  cursor.setPosition(pos);
  log_viewer_->setTextCursor(cursor);
}

}  // namespace installer