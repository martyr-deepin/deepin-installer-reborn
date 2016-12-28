// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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