// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_label.h"

#include <QDebug>

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace partman {
namespace {

TEST(PartitionLabel, ReadBtrfsLabel) {
  const QString path = "/dev/sdc5";
  const QString label = ReadLabel(path, FsType::Btrfs);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadExt4Label) {
  const QString path = "/dev/sdc6";
  const QString label = ReadLabel(path, FsType::Ext4);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadFat32Label) {
  const QString path = "/dev/sdc7";
  const QString label = ReadLabel(path, FsType::Fat32);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadHfsPlusLabel) {
  const QString path = "/dev/sdc8";
  const QString label = ReadLabel(path, FsType::HfsPlus);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadJfsLabel) {
  const QString path = "/dev/sdc9";
  const QString label = ReadLabel(path, FsType::Jfs);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadLinuxSwapLabel) {
  const QString path = "/dev/sdc10";
  const QString label = ReadLabel(path, FsType::LinuxSwap);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadNilfsLabel) {
  const QString path = "/dev/sdc11";
  const QString label = ReadLabel(path, FsType::Nilfs2);
  qDebug() << "label:" << label << ", at: " << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadNtfsLabel) {
  const QString path = "/dev/sdc12";
  const QString label = ReadLabel(path, FsType::NTFS);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadXfsLabel) {
  const QString path = "/dev/sdc13";
  const QString label = ReadLabel(path, FsType::Xfs);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadReiser4Label) {
  const QString path = "/dev/sdc14";
  const QString label = ReadLabel(path, FsType::Reiser4);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

TEST(PartitionLabel, ReadReiserfsLabel) {
  const QString path = "/dev/sdc15";
  const QString label = ReadLabel(path, FsType::Reiserfs);
  qDebug() << "label:" << label << ", at:" << path;
  EXPECT_FALSE(label.isEmpty());
}

}  // namespace
}  // namespace partman