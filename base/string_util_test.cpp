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

#include "base/string_util.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(StringUtilTest, RegexpLabel) {
  const QString uuid = RegexpLabel("UUID=([^ ]+)", "UUID=1234-321 ");
  EXPECT_EQ(uuid, "1234-321");

  const QString blocks = RegexpLabel("Free blocks:\\s*(\\d+)",
                                     "Free blocks:              5609441");
  EXPECT_EQ(blocks, "5609441");
}

}  // namespace
}  // namespace installer