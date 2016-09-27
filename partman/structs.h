// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_STRUCTS_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_STRUCTS_H

#include <QtGlobal>

// Size units defined in bytes
// See https://en.wikipedia.org/wiki/Kibibyte
const qint64 kKibiByte = 1024;
const qint64 kMebiByte = kKibiByte * kKibiByte;
const qint64 kGibiByte = kMebiByte * kKibiByte;
const qint64 kTebiByte = kGibiByte * kKibiByte;
const qint64 kPebiByte = kTebiByte * kKibiByte;
const qint64 kExbiByte = kPebiByte * kKibiByte;

const char kMountPointRoot[] = "/";
const char kMountPointBoot[] = "/boot";
const char kMountPointEFI[] = "/boot/efi";

// Unused file system and mount point.
const char kFsUnused[] = "unused";
const char kMountPointUnused[] = "unused";

namespace partman {

// This header file defines commonly used types and struct type in partman
// module.

enum class OsType {
  Empty,
  Linux,
  Mac,
  Unknown,
  Windows,
};

enum class PartitionTableType {
  Empty,  // Raw disk has empty partition table type.
  GPT,
  MsDos,
  Others,  // Not supported partition types.
  Unknown,
};

const char kPartitionTableGPT[] = "gpt";
const char kPartitionTableMsDos[] = "msdos";

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_STRUCTS_H
