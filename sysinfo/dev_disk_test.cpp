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

#include "sysinfo/dev_disk.h"

#include <QDir>
#include <QDebug>

#include "third_party/googletest/include/gtest/gtest.h"

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