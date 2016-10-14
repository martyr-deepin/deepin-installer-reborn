// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/log_manager.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>

#include "sysinfo/users.h"

namespace installer {

namespace {

// Global log file descriptor.
int g_log_fd = 0;

const char kLogFileName[] = "deepin-installer-reborn.log";

void BackupLogFile() {
  const QString log_file = GetLogFilepath();
  QFile file(log_file);
  if (file.exists()) {
    const qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    const QString old_log_file = QString("%1.%2").arg(log_file).arg(timestamp);
    file.rename(old_log_file);
  }
}

// Define customized QDebug handler.
void MessageOutput(QtMsgType type, const QMessageLogContext& context,
                   const QString& msg) {
  Q_UNUSED(type);
  Q_UNUSED(context);
  if (write(STDOUT_FILENO, msg.toUtf8().constData(),
            static_cast<size_t>(msg.length())) > -1 &&
      write(STDOUT_FILENO, "\n", 1) > -1) {
    return;
  } else {
    perror("");
  }
}

}  // namespace

QString GetLogFilepath() {
  if (HasRootPrivilege()) {
    return QString("/var/log/%1").arg(kLogFileName);
  }

  const QString tmp_log = QString("/tmp/%1").arg(kLogFileName);
  return tmp_log;
}

void RedirectLogFile() {
  qInstallMessageHandler(MessageOutput);

  BackupLogFile();

  if (g_log_fd == 0) {
    const QString log_file = GetLogFilepath();
    g_log_fd = open(log_file.toStdString().c_str(),
                    O_CREAT | O_TRUNC | O_RDWR, 0666);
    if (g_log_fd == -1) {
      qWarning() << "RedirectLogFile() failed to open " << log_file;
      g_log_fd = 0;
      return;
    }
    if (dup2(g_log_fd, STDOUT_FILENO) == -1 ||
        dup2(g_log_fd, STDERR_FILENO) == -1) {
      qWarning() << "RedirectLogFile() failed top redirect stdio";
    }
  }
}

void ShutdownLogService() {
  if (g_log_fd != 0) {
    close(g_log_fd);
  }
}

}  // namespace installer
