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

#include "partman/partition_usage.h"

#include <QDebug>
#include <QList>

#include "third_party/googletest/include/gtest/gtest.h"

namespace partman {

TEST(TestPartitionUsage, PartitionUsage) {
  QList<QPair<QString, FsType>> entries = {
//      {"/dev/sdc1", FsType::Btrfs},
//      {"/dev/sdc2", FsType::Ext2},
//      {"/dev/sdc3", FsType::Ext3},
//      {"/dev/sdc4", FsType::Ext4},
//      {"/dev/sdc5", FsType::F2fs},
//      {"/dev/sdc6", FsType::Fat32},
      {"/dev/sdc2", FsType::HfsPlus},
//      {"/dev/sdc8", FsType::Jfs},
//      {"/dev/sdc9", FsType::LinuxSwap},
//      {"/dev/sdc10", FsType::Nilfs2},
//      {"/dev/sdc11", FsType::NTFS},
//      {"/dev/sdc12", FsType::Reiser4},
//      {"/dev/sdc13", FsType::Reiserfs},
//      {"/dev/sdc14", FsType::Xfs},
  };

  for (const QPair<QString, FsType>& entry : entries) {
    qint64 freespace;
    qint64 total;
    EXPECT_TRUE(ReadUsage(entry.first, entry.second, freespace, total));
    EXPECT_GT(total, -1);
    qDebug() << "path:" << entry.first
             << ", fs:" << GetFsTypeName(entry.second)
             << ", freespace:" << freespace
             << ", total:" << total;
  }
}

}  // namespace partman