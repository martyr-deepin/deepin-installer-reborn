// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "oem/models/oem_settings_item.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#include "base/file_util.h"

namespace installer {

namespace {

const char kBooleanValue[] = "Boolean";
const char kEnumValue[] = "Enumeration";
const char kFolderValue[] = "Folder";
const char kImageFileValue[] = "ImageFile";
const char kIntegerValue[] = "Integer";
const char kStringArrayValue[] = "StringArray";
const char kStringValue[] = "String";

const char kNameField[] = "name";
const char kDescField[] = "description";
const char kTypeField[] = "type";
const char kValueField[] = "value";
const char kMinimumField[] = "minimum";
const char kMaximumField[] = "maximum";

}  // namespace

QDebug& operator<<(QDebug& debug, const OemSettingsType& type) {
  switch (type) {
    case OemSettingsType::Boolean: {
      debug << kBooleanValue;
      break;
    }
    case OemSettingsType::Enumeration: {
      debug << kEnumValue;
      break;
    }
    case OemSettingsType::Folder: {
      debug << kFolderValue;
      break;
    }
    case OemSettingsType::ImageFile: {
      debug << kImageFileValue;
      break;
    }
    case OemSettingsType::Integer: {
      debug << kIntegerValue;
      break;
    }
    case OemSettingsType::StringArray: {
      debug << kStringArrayValue;
      break;
    }
    case OemSettingsType::String: {
      debug << kStringValue;
      break;
    }
  }
  return debug;
}

OemSettingsItem::OemSettingsItem()
    : name_(),
      description_(),
      value_type_(OemSettingsType::String),
      default_value_(),
      value_(),
      minimum_(0),
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

void OemSettingsItem::setDefaultValue(const QVariant& default_value) {
  default_value_ = default_value;
}

const QVariant& OemSettingsItem::value() const {
  return value_;
}

void OemSettingsItem::setValue(const QVariant& value) {
  value_ = value;
}

qint64 OemSettingsItem::minimum() const {
  return minimum_;
}

void OemSettingsItem::setMinimum(const QVariant& minimum) {
  minimum_ = minimum.toLongLong();
}

qint64 OemSettingsItem::maximum() const {
  return maximum_;
}

void OemSettingsItem::setMaximum(const QVariant& maximum) {
  maximum_ = maximum.toLongLong();
}

QDebug& operator<<(QDebug& debug, const OemSettingsItem& item) {
  debug << "OemItem {"
        << "name:" << item.name()
        << ", description:" << item.description()
        << ", type:" << item.value_type()
        << ", default:" << item.default_value()
        << ", value:" << item.value()
        << ", minimum:" << item.minimum()
        << ", maximum:" << item.maximum()
        << "}";
  return debug;
}

QDebug& operator<<(QDebug& debug, const OemSettingsItems& items) {
  debug << "OemSettingsItems {" << "\n";
  for (const OemSettingsItem& item : items) {
    debug << "  " << item << "\n";
  }
  debug << "}";
  return debug;
}

bool DumpSettingsItems(const OemSettingsItems& items,
                       const QString& oem_json_file,
                       const QString& settings_ini_file) {
  Q_UNUSED(items);
  Q_UNUSED(oem_json_file);
  Q_UNUSED(settings_ini_file);
  return false;
}

bool LoadSettingsItems(OemSettingsItems& items,
                       const QString& default_oem_json_file,
                       const QString& oem_json_file,
                       const QString& default_settings_ini_file) {
  qDebug() << "LoadSettingsItems()" << default_oem_json_file << oem_json_file;

  const QString default_oem_content(ReadFile(default_oem_json_file));
  if (default_oem_content.isEmpty()) {
    qCritical() << "Failed to read oem item file:" << oem_json_file;
    return false;
  }

  if (!QFile::exists(default_settings_ini_file)) {
    qCritical() << "Failed to open default settings file:"
                << default_settings_ini_file;
    return false;
  }

  // Clear item list.
  items.clear();

  // First read item name, description, type and default value.
  QSettings settings(default_settings_ini_file, QSettings::IniFormat);
  const QJsonDocument default_oem_doc =
      QJsonDocument::fromJson(default_oem_content.toUtf8());
  for (const QJsonValue json_item : default_oem_doc.array()) {
    const QJsonObject obj_item = json_item.toObject();
    OemSettingsItem item;
    item.setName(obj_item.value(kNameField).toString());
    item.setDescription(obj_item.value(kDescField).toString());
    item.setValueType(obj_item.value(kTypeField).toString());

    // Read default value to ini file.
    const QVariant value(settings.value(item.name()));
    item.setDefaultValue(value);
    item.setValue(value);

    item.setMinimum(obj_item.value(kMinimumField));
    item.setMaximum(obj_item.value(kMaximumField));
    items.append(item);
  }

  // Now, update item values.
  if (!QFile::exists(oem_json_file)) {
    qDebug() << "items:" << items;
    return true;
  }
  const QString oem_content(ReadFile(oem_json_file));
  if (oem_content.isEmpty()) {
    return true;
  }

  const QJsonDocument oem_doc = QJsonDocument::fromJson(oem_content.toUtf8());
  for (const QJsonValue& json_item : oem_doc.array()) {
    const QJsonObject obj_item = json_item.toObject();
    const QString name(obj_item.value(kNameField).toString());
    for (OemSettingsItem& item : items) {
      if (item.name() == name) {
        item.setValue(obj_item.value(kValueField));
        break;
      }
    }
  }

  qDebug() << "items:" << items;

  return true;
}

}  // namespace installer