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

#ifndef INSTALLER_SYSINFO_KEYBOARD_H
#define INSTALLER_SYSINFO_KEYBOARD_H

#include <QList>
#include <QString>

namespace installer {

struct XkbModel {
  QString name = "";
  QString description = "";
  QString vendor = "";
};
typedef QList<XkbModel> XkbModelList;

struct XkbLayoutVariant {
  QString name = "";
  QString description = "";
  QString short_description = "";
  QStringList language_list;
};
typedef QList<XkbLayoutVariant> XKbLayoutVariantList;

struct XkbLayout {
  QString name = "";
  QString description = "";
  QString short_description = "";
  QStringList language_list;
  XKbLayoutVariantList variant_list;
};
typedef QList<XkbLayout> XkbLayoutList;

struct XkbConfig {
  XkbModelList model_list;
  XkbLayoutList layout_list;
};

// Read system keyboard layout, with localized description.
// |locale| is the desired locale name.
XkbConfig GetXkbConfig(const QString& locale);

// Set current keyboard layout to |layout|.
bool SetXkbLayout(const QString& layout);

// Set current keyboard |layout| and |variant|.
bool SetXkbLayout(const QString& layout, const QString& variant);

// Set current keyboard |model|.
bool SetXkbModel(const QString& model);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_KEYBOARD_H
