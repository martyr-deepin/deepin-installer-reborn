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

#include "unsquashfs_progress_window.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QThreadPool>
#include <QTimer>

#include "base/command.h"
#include "base/file_util.h"
#include "worker.h"

namespace {

const char kSelectText[] = "Select squashfs file";

const char kUnsquashfsBaseProgressFile[] = "/dev/shm/unsquashfs_progress";
// Interval to read unsquashfs progress file, 500ms.
const int kReadUnsquashfsInterval = 500;

int ReadProgressValue(const QString& file) {
  if (QFile::exists(file)) {
    const QString val(installer::ReadFile(file));
    qDebug() << val;
    if (!val.isEmpty()) {
      return val.toInt();
    }
  }
  return 0;
}

}  // namespace

UnsquashfsProgressWindow::UnsquashfsProgressWindow(QWidget* parent)
    : QFrame(parent),
      work_running_(false) {
  this->setObjectName("unsquashfs_progress");

  timer_ = new QTimer(this);
  timer_->setInterval(kReadUnsquashfsInterval);

  this->initUI();
  this->initConnections();
}

void UnsquashfsProgressWindow::initConnections() {
  connect(ctrl_button_, &QPushButton::clicked,
          this, &UnsquashfsProgressWindow::onCtrlButtonClicked);
  connect(timer_, &QTimer::timeout,
          this, &UnsquashfsProgressWindow::onReadProgressTimeout);
}

void UnsquashfsProgressWindow::initUI() {
  filepath_label_ = new QLabel(this);
  ctrl_button_ = new QPushButton(kSelectText, this);
  progress_bar_ = new QProgressBar(this);
  progress_bar_->setOrientation(Qt::Horizontal);
  progress_bar_->setTextVisible(true);

  QGridLayout* layout = new QGridLayout(this);
  layout->addWidget(ctrl_button_, 0, 0);
  layout->addWidget(filepath_label_, 1, 0);
  layout->addWidget(progress_bar_, 1, 0, 1, 2);
  this->setLayout(layout);

  this->resize(640, 480);
  this->setWindowTitle("Unsquashfs Progress Window");
}

void UnsquashfsProgressWindow::startWork(const QString& filepath) {
  work_running_ = true;
  ctrl_button_->setText("Abort");

  // Remove old progress file first.
  QFile::remove(kUnsquashfsBaseProgressFile);

  timer_->start();

  // Start a new worker.
  Worker* worker = new Worker(filepath);
  worker->setAutoDelete(true);
  QThreadPool::globalInstance()->start(worker);
}

void UnsquashfsProgressWindow::cancelWork() {
  // TODO(xushaohua): stop worker.
  work_running_ = false;
  ctrl_button_->setText(kSelectText);
  progress_bar_->reset();
  timer_->stop();
}

void UnsquashfsProgressWindow::onCtrlButtonClicked() {
  if (work_running_) {
    // Work is running, so that it is cancellable.
    this->cancelWork();
  } else {
    // Show file chooser dialog.
    const QString filepath = QFileDialog::getOpenFileName(
        this, "Select squashfs file",
        QDir::homePath(), "Squashfs Files (*.squashfs)");
    if (!filepath.isEmpty()) {
      ctrl_button_->setText("Cancel");
      filepath_label_->setText(filepath);
      this->startWork(filepath);
    }
  }
}

void UnsquashfsProgressWindow::onReadProgressTimeout() {
  const int progress = ReadProgressValue(kUnsquashfsBaseProgressFile);
  progress_bar_->setValue(progress);
}
