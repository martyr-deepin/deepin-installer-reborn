// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/log_manager.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <QFile>
#include <QFileInfo>
#include <QtGlobal>

#include "sysinfo/users.h"

#include "third_party/CuteLogger/CuteLogger/ConsoleAppender.h"
#include "third_party/CuteLogger/CuteLogger/FileAppender.h"
#include "third_party/CuteLogger/CuteLogger/Logger.h"

namespace installer {

namespace {

// Defines log format.
const char kDebugLogFormat[] =
    "[%{type:-7}] [%{file:-25} %{line}] %{message}\n";

const char kReleaseLogFormat[] = "%{message}\n";

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

}  // namespace

QString GetLogFilepath() {
  if (HasRootPrivilege()) {
    return QString("/var/log/%1").arg(kLogFileName);
  }

  const QString tmp_log = QString("/tmp/%1").arg(kLogFileName);
  return tmp_log;
}

void InitLogService() {
  BackupLogFile();

  // TODO(xushaohua): Release log appender.
  ConsoleAppender* console_appender = new ConsoleAppender();
  console_appender->setDetailsLevel(Logger::Debug);
  logger->registerAppender(console_appender);
  console_appender->setFormat(kDebugLogFormat);

  FileAppender* file_appender = new FileAppender(GetLogFilepath());
  file_appender->setDetailsLevel(Logger::Debug);
  file_appender->setFormat(kReleaseLogFormat);
  logger->registerAppender(file_appender);
}

}  // namespace installer
