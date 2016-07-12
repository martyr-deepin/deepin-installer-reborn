// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/log_manager.h"

#include <QFile>
#include "third_party/CuteLogger/CuteLogger/ConsoleAppender.h"
#include "third_party/CuteLogger/CuteLogger/FileAppender.h"
#include "third_party/CuteLogger/CuteLogger/Logger.h"

namespace service {

namespace {

// Defines log format.
const char kLogFormat[] =
    "%{time}{MM-dd, HH:mm:ss} [%{type:-7}] [%{file:-25} %{line}] %{message}\n";

const char kLogFileName[] = "deepin-installer-reborn.log";

}  // namespace

QString GetLogFilepath() {
  const QString var_log = QStringLiteral("/var/log/%1").arg(kLogFileName);
  QFile var_log_file(var_log);
  if (var_log_file.isWritable()) {
    if (var_log_file.exists()) {
      var_log_file.remove();
    }
    return var_log;
  }

  const QString tmp_log = QStringLiteral("/tmp/%1").arg(kLogFileName);
  QFile tmp_log_file(tmp_log);
  if (tmp_log_file.exists()) {
    tmp_log_file.remove();
  }
  return tmp_log;
}

void InitLogService() {
  // TODO(xushaohua): Free console_appender and file_appender.
  ConsoleAppender* console_appender = new ConsoleAppender();
#ifndef NDEBUG
  console_appender->setDetailsLevel(Logger::Debug);
#else
  console_appender->setDetailsLevel(Logger::Warning);
#endif
  console_appender->setFormat(kLogFormat);
  logger->registerAppender(console_appender);

  FileAppender* file_appender = new FileAppender(GetLogFilepath());
#ifndef NDEBUG
  file_appender->setDetailsLevel(Logger::Debug);
#else
  file_appender->setDetailsLevel(Logger::Warning);
#endif
  file_appender->setFormat(kLogFormat);
  logger->registerAppender(file_appender);
}

}  // namespace service