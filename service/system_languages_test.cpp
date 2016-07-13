// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/system_language.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace service {
namespace {

TEST(SystemLanguageTest, GetLanguageList) {
  const LanguageList lang_list= GetLanguageList();
  EXPECT_TRUE(lang_list.length() > 0);
  QStringList locale_list;
  for (const LanguageItem& lang : lang_list) {
    locale_list.append(lang.locale);
  }
  EXPECT_TRUE(locale_list.contains("en_US"));
}

}  // namespace
}  // namespace service