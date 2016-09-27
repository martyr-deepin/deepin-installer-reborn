// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_label.h"

#include <QDebug>

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace partman {
namespace {

TEST(PartitionLabel, ReadLabel) {
  const QStringList paths = {"/dev/sdc1", "/dev/sdc2", "/dev/sdc3"};
  for (const QString& path : paths) {
    const QString label = ReadLabel(path);
    qDebug() << "label:" << label << ", at:" << path;
    EXPECT_FALSE(label.isEmpty());
  }
}

}  // namespace
}  // namespace partman