// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/dev_disk.h"

#include <QDir>
#include <QDebug>

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(DevDiskTest, ParseLabelDir) {
  QDir dir("/dev/disk/by-label");
  const QStringList entries = dir.entryList(QDir::NoDotAndDotDot);
  const LabelItems label_items = ParseLabelDir();
  EXPECT_EQ(entries.size(), label_items.size());
}

TEST(DevDiskTest, ParsePartLabelDir) {
  QDir dir("/dev/disk/by-partlabel");
  const QStringList entries = dir.entryList(QDir::NoDotAndDotDot);
  const LabelItems label_items = ParsePartLabelDir();
  EXPECT_EQ(entries.size(), label_items.size());
}

TEST(DevDiskTest, ParseUUIDDir) {
  QDir dir("/dev/disk/by-uuid");
  const QStringList entries = dir.entryList(QDir::NoDotAndDotDot);
  const LabelItems label_items = ParseUUIDDir();
  EXPECT_EQ(entries.size(), label_items.size());
}

}  // namespace
}  // namespace installer