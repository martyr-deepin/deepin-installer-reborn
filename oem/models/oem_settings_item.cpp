// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "oem/models/oem_settings_item.h"

#include <QDebug>

namespace installer {

namespace {

const char kBooleanValue[] = "Boolean";
const char kEnumValue[] = "Enumeration";
const char kFolderValue[] = "Folder";
const char kImageFileValue[] = "ImageFile";
const char kIntegerValue[] = "Integer";
const char kStringArrayValue[] = "StringArray";
const char kStringValue[] = "String";

}  // namespace

OemSettingsItem::OemSettingsItem()
    : name_(),
      description_(),
      value_type_(OemSettingsType::String),
      default_value_(),
      value_(),
      has_minimum_(false),
      minimum_(0),
      has_maximum_(false),
      maximum_(0) {
}

const QString& OemSettingsItem::name() const {
  return name_;
}

void OemSettingsItem::setName(const QString& name) {
  name_ = name;
}

const QString& OemSettingsItem::description() const {
  return description_;
}

void OemSettingsItem::setDescription(const QString& description) {
  description_ = description;
}

OemSettingsType OemSettingsItem::value_type() const {
  return value_type_;
}

void OemSettingsItem::setValueType(const QString& value_type) {
  if (value_type == kBooleanValue) {
    value_type_ = OemSettingsType::Boolean;
  } else if (value_type == kEnumValue) {
    value_type_ = OemSettingsType::Enumeration;
  } else if (value_type == kFolderValue) {
    value_type_ = OemSettingsType::Folder;
  } else if (value_type == kImageFileValue) {
    value_type_ = OemSettingsType::ImageFile;
  } else if (value_type == kIntegerValue) {
    value_type_ = OemSettingsType::Integer;
  } else if (value_type == kStringArrayValue) {
    value_type_ = OemSettingsType::StringArray;
  } else if (value_type == kStringValue) {
    value_type_ = OemSettingsType::String;
  } else {
    qWarning() << "Unsupported value type, treat as String" << value_type;
    value_type_ = OemSettingsType::String;
  }
}

const QVariant& OemSettingsItem::default_value() const {
  return default_value_;
}

void OemSettingsItem::setDefaultValue(const QString& default_value) {
  default_value_ = default_value;
}

const QVariant& OemSettingsItem::value() const {
  return value_;
}

void OemSettingsItem::setValue(const QString& value) {
  value_ = value;
}

bool OemSettingsItem::hasMinimum() const {
  return has_minimum_;
}

qint64 OemSettingsItem::minimum() const {
  return minimum_;
}

void OemSettingsItem::setMinimum(const QString& minimum) {
  has_minimum_ = true;
  minimum_ = minimum.toLongLong();
}

bool OemSettingsItem::hasMaximum() const {
  return has_maximum_;
}

qint64 OemSettingsItem::maximum() const {
  return maximum_;
}

void OemSettingsItem::setMaximum(const QString& maximum) {
  has_maximum_ = true;
  maximum_ = maximum.toLongLong();
}

}  // namespace installer