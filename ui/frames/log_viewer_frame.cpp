// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/log_viewer_frame.h"

#include <QTextEdit>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/log_manager.h"

namespace ui {

namespace {

// Read log file each 1000ms.
const int kReadLogInterval = 1000;

}

LogViewerFrame::LogViewerFrame(QWidget* parent)
    : QFrame(parent),
      timer_(this),
      log_file_path_(service::GetLogFilepath()),
      log_content_() {
  this->setObjectName(QStringLiteral("log_viewer_frame"));

  timer_.setInterval(kReadLogInterval);
  this->initUI();
  this->initConnection();
}

void LogViewerFrame::toggleVisible() {
  setVisible(!isVisible());
}

void LogViewerFrame::hideEvent(QHideEvent* event) {
  timer_.stop();
  QWidget::hideEvent(event);
}

void LogViewerFrame::showEvent(QShowEvent* event) {
  // Read log file immediately.
  this->onTimerTimeout();
  timer_.start();
  QWidget::showEvent(event);
}

void LogViewerFrame::initConnection() {
  connect(&timer_, &QTimer::timeout,
          this, &LogViewerFrame::onTimerTimeout);
}

void LogViewerFrame::initUI() {
  text_edit_ = new QTextEdit();
  // Disable user modification.
  text_edit_->setReadOnly(true);
  // Disable rich text support.
  text_edit_->setAcceptRichText(false);
  // Disable context menu.
  text_edit_->setContextMenuPolicy(Qt::NoContextMenu);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(text_edit_);
  this->setLayout(layout);

  this->setStyleSheet(
      base::ReadTextFileContent(":/styles/log_viewer_frame.css"));
  this->setWindowFlags(Qt::FramelessWindowHint);
  this->setWindowModality(Qt::WindowModal);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  this->resize(860, 640);
}

void LogViewerFrame::onTimerTimeout() {
  const QString content = base::ReadTextFileContent(log_file_path_);
  if (content == log_content_) {
    return;
  }
  log_content_ = content;
  text_edit_->setPlainText(log_content_);
}

}  // namespace ui