// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/install_slide_frame_util.h"

#include <QDebug>
#include <QStringList>

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(InstallSlideFrameUtil, GetSlideDir) {
  EXPECT_TRUE(GetSlideDir("zh_CN").endsWith("zh_CN"));
  EXPECT_TRUE(GetSlideDir("ru_RU").endsWith("default"));
}

TEST(InstallSlideFrameUtil, GetSlideFiles) {
  EXPECT_FALSE(GetSlideFiles("en_US").isEmpty());
  EXPECT_FALSE(GetSlideFiles("zh_CN").isEmpty());
}

}  // namespace
}  // namespace installer