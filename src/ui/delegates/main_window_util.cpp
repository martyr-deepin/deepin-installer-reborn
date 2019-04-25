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

#include "ui/delegates/main_window_util.h"

#include "base/consts.h"
#include "base/file_util.h"
#include "partman/partition.h"
#include "partman/utils.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/release_version.h"
#include "third_party/zlib_wrapper/zip_util.h"
#include "ui/delegates/partition_util.h"

namespace installer {

void CopyLogFile(const QString& log_file) {
  const QString current_log_file(GetLogFilepath());
  if (!CopyFile(current_log_file, log_file, true)) {
    qCritical() << "Failed to copy log to:" << log_file;
  }
}

// Encode |msg| with base64()
QString EncodeErrorMsg(const QString& msg) {
  const QString prefix = GetSettingsString(kInstallFailedFeedbackServer);
  QString encoded_msg;
  const QByteArray msg_bytes = msg.toUtf8();
  QByteArray zip_output;
  // Try gzip first, if failed use base64 only.
  if (GzipCompress(msg_bytes, zip_output, 9)) {
    QString fuck(zip_output.constData());
    const QByteArray b64 = zip_output.toBase64();
    encoded_msg = prefix.arg(b64.constData());
  } else {
    qWarning() << "GZipCompress failed, fallback to b64";
    // Use only part of msg if zip-compress is failed.
    const int tail_msg_len = msg.length() * 3 / 4;
    const QString tail_msg = msg.right(tail_msg_len);
    const QByteArray b64 = tail_msg.toUtf8().toBase64();
    encoded_msg = prefix.arg(QString::fromUtf8(b64));
  }
  return encoded_msg;
}

int GetVisiblePages() {
  int pages = 0;
  if (!GetSettingsBool(kSkipSelectLanguagePage)) {
    pages += 1;
  }

  if (!GetSettingsBool(kSystemInfoSetupAfterReboot) || !GetSettingsBool(kSkipSystemInfoPage)) {
    pages += 1;
  }
  if (!GetSettingsBool(kSystemInfoSetupAfterReboot) || !GetSettingsBool(kSkipTimezonePage)) {
    pages += 1;
  }
  if (!GetSettingsBool(kSkipPartitionPage)) {
    pages += 1;
  }
  // For install-progress page.
  pages += 1;
  return pages;
}

bool IsDiskSpaceInsufficient() {
  const int minimum = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
  const qint64 maximum_device_size = GetMaximumDeviceSize();
  return minimum * kGibiByte > maximum_device_size;
}

bool ReadErrorMsg(QString& msg, QString& encoded_msg) {
  const QString raw_msg = ReadFile(GetLogFilepath());
  if (raw_msg.isEmpty()) {
    qCritical() << "log file is empty!" << GetLogFilepath();
    return false;
  }

  const QString stripped_msg =
      raw_msg.right(GetSettingsInt(kInstallFailedErrMsgLen));
  const ReleaseVersion version = GetReleaseVersioin();
  msg = QString("%1\n%2, %3, %4")
      .arg(stripped_msg)
      .arg(kAppVersion)
      .arg(version.version)
      .arg(version.type);
  const int qr_max_len = GetSettingsInt(kInstallFailedQRErrMsgLen);
  encoded_msg = EncodeErrorMsg(raw_msg.right(qr_max_len));
  return true;
}

}  // namespace installer
