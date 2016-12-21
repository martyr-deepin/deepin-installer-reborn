// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
// Sort keyboard variant by description.
bool operator<(const XkbLayoutVariant& a, const XkbLayoutVariant& b);
typedef QList<XkbLayoutVariant> XKbLayoutVariantList;

struct XkbLayout {
  QString name = "";
  QString description = "";
  QString short_description = "";
  QStringList language_list;
  XKbLayoutVariantList variant_list;
};
// Sort XkbLayout by description.
bool operator<(const XkbLayout& a, const XkbLayout& b);
typedef QList<XkbLayout> XkbLayoutList;

struct XkbConfig {
  XkbModelList model_list;
  XkbLayoutList layout_list;
};

// Read system keyboard layout.
// TODO(xushaohua): Call gettext()
XkbConfig GetXkbConfig();

// Set current keyboard layout to |layout|.
bool SetXkbLayout(const QString& layout);

// Set current keyboard |layout| and |variant|.
bool SetXkbLayout(const QString& layout, const QString& variant);

// Set current keyboard |model|.
bool SetXkbModel(const QString& model);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_KEYBOARD_H
