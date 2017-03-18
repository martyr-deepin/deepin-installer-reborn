// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_OEM_MODELS_OEM_SETTINGS_ITEM_H
#define INSTALLER_OEM_MODELS_OEM_SETTINGS_ITEM_H

#include <QDebug>
#include <QList>
#include <QString>
#include <QVariant>
#include <QVector>

namespace installer {

// Value type in each oem settings item.
enum class OemSettingsType {
  Base64String,
  Boolean,
  Enumeration,
  Folder,
  ImageFile,
  Integer,
  // If value type if undefined, treat it as string value.
  String,
  StringArray,
};
QString OemSettingsTypeToString(OemSettingsType type);
QDebug& operator<<(QDebug& debug, const OemSettingsType& type);

// A struct to represent each oem settings item.
// UI layout will be drawn based on its value type and other properties.
struct OemSettingsItem {
 public:
  OemSettingsItem();
  // TODO(xushaohua): Add other constructors.

  const QString title() const;
  void setTitle(const QString& title);

  const QString& name() const;
  void setName(const QString& name);

  const QString& description() const;
  void setDescription(const QString& description);

  // Set value type. Might be "Boolean", "Integer" or other defined types.
  // If |value_type| is not supported, treat it as plain "String".
  OemSettingsType value_type() const;
  void setValueType(const QString& value_type);

  // Get/set default_value.
  // Note that value_type shall be set before updating default value.
  const QVariant& default_value() const;
  void setDefaultValue(const QVariant& default_value);

  // Get/set current value.
  // Note that value_type shall be set before updating item value.
  const QVariant& value() const;
  void setValue(const QVariant& value);

  // Get/set/check minimum and maximum property.
  // These properties are optional, and it only makes sense only if
  // value_type_ is Integer.
  int minimum() const;
  void setMinimum(const QVariant& minimum);
  int maximum() const;
  void setMaximum(const QVariant& maximum);

 private:
  QString title_;
  QString name_;
  QString description_;
  OemSettingsType value_type_;
  QVariant default_value_;
  QVariant value_;
  int minimum_;
  int maximum_;
};
QDebug& operator<<(QDebug& debug, const OemSettingsItem& item);
typedef QVector<OemSettingsItem> OemSettingsItems;
QDebug& operator<<(QDebug& debug, const OemSettingsItems& items);

// Dump oem setting |items| to disk.
// |settings_ini_file| is path to user customized settings file (value).
bool DumpSettingsItems(const OemSettingsItems& items,
                       const QString& settings_ini_file);

// Read oem settings and update to |items|.
// |default_oem_json_file| is path to system oem json file.
// |default_settings_ini_file| is path to system settings ini file.
// |settings_ini_file| is path to user customized settings file (value).
bool LoadSettingsItems(OemSettingsItems& items,
                       const QString& default_oem_json_file,
                       const QString& default_settings_ini_file,
                       const QString& settings_ini_file);

}  // namespace installer

#endif  // INSTALLER_OEM_MODELS_OEM_SETTINGS_ITEM_H
