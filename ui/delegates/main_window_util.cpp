// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/main_window_util.h"

#include "base/file_util.h"
#include "partman/partition.h"
#include "partman/utils.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "third_party/zlib_wrapper/zip_util.h"
#include "ui/delegates/partition_util.h"

namespace installer {

// Encode |msg| with base64()
QString EncodeErrorMsg(const QString& msg) {
  const QString base64_content = msg.toUtf8().toBase64();
  const QByteArray b64 = msg.toUtf8().toBase64();
  QByteArray zip_output;
  const QString prefix = GetSettingsString(kInstallFailedFeedbackServer);
  QString encoded_msg;

  // Try gzip first, if failed use base64 only.
  if (GzipCompress(b64, zip_output, 9)) {
    encoded_msg = prefix.arg(QString::fromUtf8(zip_output));
  } else {
    encoded_msg = prefix.arg(QString::fromUtf8(b64));
  }
  return encoded_msg;
}

int GetVisiblePages() {
  int pages = 0;
  if (!GetSettingsBool(kSkipSelectLanguagePage)) {
    pages += 1;
  }
  if (!GetSettingsBool(kSkipSystemInfoPage)) {
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
  int minimum = 0;
  // Check whether memory is too small.
  if (IsSwapAreaNeeded()) {
    minimum = GetSettingsInt(kPartitionMinimumDiskSpaceRequiredInLowMemory);
  } else {
    minimum = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
  }

  const qint64 maximum_device_size = GetMaximumDeviceSize();
  return minimum * kGibiByte > maximum_device_size;
}

bool IsPartitionTableMatch() {
  // If EFI is not enabled, always returns true.
  if (!IsEfiEnabled()) {
    return true;
  }

  const PartitionTableType type = GetPrimaryDiskPartitionTable();
  // If partition table is empty(a raw disk device), returns true.
  if (type == PartitionTableType::Empty) {
    return true;
  }

  return type == PartitionTableType::GPT;
}

bool ReadErrorMsg(QString& msg, QString& encoded_msg) {
  const QString raw_msg = ReadFile(GetLogFilepath());
  if (raw_msg.isEmpty()) {
    qCritical() << "log file is empty!" << GetLogFilepath();
    return false;
  }

  msg = raw_msg.right(GetSettingsInt(kInstallFailedErrMsgLen));
  const int qr_max_len = GetSettingsInt(kInstallFailedQRErrMsgLen);
  encoded_msg = EncodeErrorMsg(raw_msg.right(qr_max_len));
  return true;
}

bool SaveLogFileToDisk() {
  const QString filepath(GetSettingsString(kDiskLogFilePath));
  if (filepath.isEmpty()) {
    return false;
  } else {
    return QFile::copy(GetLogFilepath(), filepath);
  }
}

}  // namespace installer