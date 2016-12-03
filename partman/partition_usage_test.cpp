// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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