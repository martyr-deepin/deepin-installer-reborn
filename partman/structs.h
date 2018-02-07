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

#ifndef INSTALLER_PARTMAN_STRUCTS_H
#define INSTALLER_PARTMAN_STRUCTS_H

#include <QDebug>
#include <QtGlobal>

namespace installer {

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

// This header file defines commonly used types and struct type in partman
// module.

enum class OsType {
  Empty,
  Linux,
  Mac,
  Unknown,
  Windows,
};
QDebug& operator<<(QDebug& debug, const OsType& os_type);

enum class PartitionTableType {
  Empty,  // Raw disk has empty partition table type.
  GPT,
  MsDos,
  Others,  // Not supported partition types.
  Unknown,
};
QDebug& operator<<(QDebug& debug, const PartitionTableType& table_type);
const char* GetPartTableName(PartitionTableType table_type);

const char kPartitionTableGPT[] = "gpt";
const char kPartitionTableMsDos[] = "msdos";

// Number of partitions supported by these partition tables.
const int kGPTPartitionNums = 128;
const int kMsDosPartitionNums = 4;

}  // namespace installer

#endif  // INSTALLER_PARTMAN_STRUCTS_H
