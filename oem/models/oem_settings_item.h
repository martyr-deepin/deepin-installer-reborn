// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_OEM_MODELS_OEM_SETTINGS_ITEM_H
#define INSTALLER_OEM_MODELS_OEM_SETTINGS_ITEM_H

#include <QList>
#include <QString>
#include <QVariant>

namespace installer {

// Value type in each oem settings item.
enum class OemSettingsType {
  Boolean,
  Enumeration,
  Folder,
  ImageFile,
  Integer,
  // If value type if undefined, treat it as string value.
      String,
  StringArray,
};

// A struct to represent each oem settings item.
// UI layout will be drawn based on its value type and other properties.
struct OemSettingsItem {
 public:
  OemSettingsItem();
  // TODO(xushaohua): Add other constructors.

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
  void setDefaultValue(const QString& default_value);

  // Get/set current value.
  // Note that value_type shall be set before updating item value.
  const QVariant& value() const;
  void setValue(const QString& value);

  // Get/set/check minimum and maximum property.
  // These properties are optional, so check whether they have been set before
  // reading their values.
  // These properties only make sense only if value_type_ is Integer.
  bool hasMinimum() const;
  qint64 minimum() const;
  void setMinimum(const QString& minimum);
  bool hasMaximum() const;
  qint64 maximum() const;
  void setMaximum(const QString& maximum);

 private:
  QString name_;
  QString description_;
  OemSettingsType value_type_;
  QVariant default_value_;
  QVariant value_;
  bool has_minimum_;
  qint64 minimum_;
  bool has_maximum_;
  qint64 maximum_;
};

}  // namespace installer

#endif  // INSTALLER_OEM_MODELS_OEM_SETTINGS_ITEM_H
