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

#include "oem/models/oem_settings_item.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#include "base/file_util.h"
#include "base/string_util.h"
#include "service/system_language.h"

namespace installer {

namespace {

const char kBase64StringValue[] = "Base64String";
const char kBooleanValue[] = "Boolean";
const char kEnumValue[] = "Enumeration";
const char kFolderValue[] = "Folder";
const char kImageFileValue[] = "ImageFile";
const char kIntegerValue[] = "Integer";
const char kStringArrayValue[] = "StringArray";
const char kStringValue[] = "String";

// Field name defined in oem json file.
const char kTitleField[] = "title";
const char kNameField[] = "name";
const char kDescField[] = "description";
const char kTypeField[] = "type";
const char kMinimumField[] = "minimum";
const char kMaximumField[] = "maximum";

const char kLanguageItemName[] = "select_language_default_locale";

// Update language list options_ in |items|.
void UpdateLanguageListOptions(OemSettingsItems& items) {
  int index = -1;
  for (index = 0; index < items.length(); ++index) {
    if (items.at(index).name() == kLanguageItemName) {
      break;
    }
  }

  if (index == items.length()) {
    qWarning() << "Failed to find language item:" << kLanguageItemName;
    return;
  }

  const LanguageList languages = GetLanguageList();
  QStringList locales;
  for (const LanguageItem& language : languages) {
    locales.append(language.locale);
  }

  // Update options.
  items[index].setOptions(locales);
}

}  // namespace

QString OemSettingsTypeToString(OemSettingsType type) {
  switch (type) {
    case OemSettingsType::Base64String: {
      return kBase64StringValue;
    }
    case OemSettingsType::Boolean: {
      return kBooleanValue;
    }
    case OemSettingsType::Enumeration: {
      return kEnumValue;
    }
    case OemSettingsType::Folder: {
      return kFolderValue;
    }
    case OemSettingsType::ImageFile: {
      return kImageFileValue;
    }
    case OemSettingsType::Integer: {
      return kIntegerValue;
    }
    case OemSettingsType::StringArray: {
      return kStringArrayValue;
    }
    case OemSettingsType::String: {
      return kStringValue;
    }
    default: {
      return "";
    }
  }
}

QDebug& operator<<(QDebug& debug, const OemSettingsType& type) {
  debug << OemSettingsTypeToString(type);
  return debug;
}

OemSettingsItem::OemSettingsItem()
    : title_(),
      name_(),
      description_(),
      value_type_(OemSettingsType::String),
      default_value_(),
      value_(),
      minimum_(0),
      maximum_(0) {
}

const QString& OemSettingsItem::title() const {
  return title_;
}

void OemSettingsItem::setTitle(const QString& title) {
  title_ = title;
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
  if (value_type == kBase64StringValue) {
    value_type_ = OemSettingsType::Base64String;
  } else if (value_type == kBooleanValue) {
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

const QStringList& OemSettingsItem::options() const {
  return options_;
}

void OemSettingsItem::setOptions(const QStringList& options) {
  options_ = options;
}

int OemSettingsItem::minimum() const {
  return minimum_;
}

void OemSettingsItem::setMinimum(const QVariant& minimum) {
  minimum_ = minimum.toInt();
}

int OemSettingsItem::maximum() const {
  return maximum_;
}

void OemSettingsItem::setMaximum(const QVariant& maximum) {
  maximum_ = maximum.toInt();
}

QDebug& operator<<(QDebug& debug, const OemSettingsItem& item) {
  debug << "OemItem {"
        << "title:" << item.title()
        << ", name:" << item.name()
        << ", description:" << item.description()
        << ", type:" << item.value_type()
        << ", default:" << item.default_value()
        << ", value:" << item.value()
        << ", options:" << item.options()
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

void DumpSettingsItems(const OemSettingsItems& items,
                       const QString& settings_ini_file) {
  qDebug() << "DumpItems:" << settings_ini_file;
  CreateParentDirs(settings_ini_file);

  QSettings settings(settings_ini_file, QSettings::IniFormat);
  for (const OemSettingsItem& item : items) {
    const QString& item_name = item.name();
    if (item.value() != item.default_value()) {
      if (item.value_type() == OemSettingsType::Base64String) {
        const QString value_str = Base64Encode(item.value().toString());
        settings.setValue(item_name, value_str);
      } else {
        settings.setValue(item_name, item.value());
      }
    } else if (settings.contains(item_name)) {
      // Remove item from settings if its value equals to default value.
      settings.remove(item_name);
    }
  }
}

bool LoadSettingsItems(OemSettingsItems& items,
                       const QString& default_oem_json_file,
                       const QString& default_settings_ini_file,
                       const QString& settings_ini_file) {
  const QString default_oem_content(ReadFile(default_oem_json_file));
  if (default_oem_content.isEmpty()) {
    qCritical() << "Failed to read oem item file:" << default_oem_json_file;
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
  QSettings default_settings(default_settings_ini_file, QSettings::IniFormat);
  QSettings settings(settings_ini_file, QSettings::IniFormat);
  const QJsonDocument default_oem_doc =
      QJsonDocument::fromJson(default_oem_content.toUtf8());
  for (const QJsonValue json_item : default_oem_doc.array()) {
    const QJsonObject obj_item = json_item.toObject();
    OemSettingsItem item;
    item.setTitle(obj_item.value(kTitleField).toString());
    const QString item_name = obj_item.value(kNameField).toString();
    item.setName(item_name);
    item.setDescription(obj_item.value(kDescField).toString());
    item.setValueType(obj_item.value(kTypeField).toString());

    // Read default value to default settings ini file.
    if (!default_settings.contains(item_name)) {
      qWarning() << "Invalid key:" << item_name;
    }

    if (item.value_type() == OemSettingsType::Base64String) {
      const QVariant default_value(default_settings.value(item_name));
      const QString default_value_str = Base64Decode(default_value.toString());
      item.setDefaultValue(default_value_str);

      // Read customized value from settings ini file if found.
      if (settings.contains(item_name)) {
        const QVariant value = settings.value(item_name);
        const QString value_str = Base64Decode(value.toString());
        item.setValue(value_str);
      } else {
        item.setValue(default_value_str);
      }
    } else {
      const QVariant default_value(default_settings.value(item_name));
      item.setDefaultValue(default_value);

      // Read customized value from settings ini file if found.
      if (settings.contains(item_name)) {
        item.setValue(settings.value(item_name));
      } else {
        item.setValue(default_value);
      }
    }

    if (item.value_type() == OemSettingsType::Integer) {
      item.setMinimum(obj_item.value(kMinimumField));
      item.setMaximum(obj_item.value(kMaximumField));
    }
    items.append(item);
  }

  // Load language list.
  UpdateLanguageListOptions(items);

  return true;
}

}  // namespace installer