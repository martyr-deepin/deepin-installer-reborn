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