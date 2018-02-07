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

#include "sysinfo/iso3166.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(Iso3166Test, GetCountryEntries) {
  const CountryEntries entries = GetCountryEntries();
  EXPECT_GT(entries.length(), 10);
}

TEST(Iso3166Test, GetCountryEntryIndex) {
  const CountryEntries entries = GetCountryEntries();
  const int cn_index = GetCountryEntryIndex(entries, "CN");
  EXPECT_GT(cn_index, 0);
}

}  // namespace
}  // namespace installer