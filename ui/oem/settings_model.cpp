// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/oem/settings_model.h"

#include <QCheckBox>
#include <QDebug>
#include <QDir>
#include <QLineEdit>
#include <QSettings>
#include <QSlider>

#include "service/settings_name.h"

namespace installer {

namespace {

const char kSettingsName[] = "settings.ini";

const char kDefaultSettingsFile[] = RESOURCES_DIR "/default_settings.ini";

// Returns absolute path to $HOME/oem/ folder.
QString GetOemFolder() {
  return QDir::home().absoluteFilePath("oem");
}

QVariant GetSettingsValue(const QString& key);

bool GetSettingsBool(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toBool();
  }

  qCritical() << "GetSettingsBool() failed with key:" << key;
  return false;
}

int GetSettingsInt(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toInt();
  }

  qCritical() << "GetSettingsInt() failed with key:" << key;
  return 0;
}

QString GetSettingsString(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toString();
  }

  qCritical() << "GetSettingsString() failed with key:" << key;
  return QString();
}

//QStringList GetSettingsStringList(const QString& key) {
//  const QVariant value = GetSettingsValue(key);
//  if (!value.isValid()) {
//    qCritical() << "GetSettingsStringList() failed with key:" << key;
//    return QStringList();
//  }
//
//  return value.toString().split(';');
//}

QVariant GetSettingsValue(const QString& key) {
  QDir oem_dir(GetOemFolder());
  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  QSettings settings(settings_file, QSettings::IniFormat);
  const QVariant value(settings.value(key));
  if (!value.isValid()) {
    qWarning() << "getSettingsValue() Invalid key:" << key;
  }
  return value;
}

// Write settings to file.
void SetSettingsValue(const QString& key, const QVariant& value) {
  QDir oem_dir(GetOemFolder());
  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  QSettings settings(settings_file, QSettings::IniFormat);
  settings.setValue(key, value);
}

}  // namespace

SettingsModel::SettingsModel(QObject* parent) : QObject(parent) {
  this->setObjectName("settings_model");
}

bool SettingsModel::initOemFolder() {
  QDir oem_dir(GetOemFolder());

  if (!oem_dir.exists()) {
    if (!oem_dir.mkpath(".")) {
      qWarning() << "Failed to create oem dir" << oem_dir.absolutePath();
      return false;
    }
  }

  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  QSettings oem_settings(settings_file, QSettings::IniFormat);
  QSettings default_settings(kDefaultSettingsFile, QSettings::IniFormat);
  for (const QString& key : default_settings.allKeys()) {
    // Merge default settings. Do not use section groups.
    if (!oem_settings.contains(key)) {
      const QVariant value(default_settings.value(key));
      oem_settings.setValue(key, value);
    }
  }

  return true;
}

QCheckBox* SettingsModel::addCheckBox(const QString& property) {
  QCheckBox* check_box = new QCheckBox();
  check_box->setObjectName("check_box");

  // Save property name to widget's property-list.
  check_box->setChecked(GetSettingsBool(property));

  // Read current property value.
  check_box->setProperty(kSettingsPropName, property);

  // Watch for changes.
  connect(check_box, &QCheckBox::toggled,
          this, &SettingsModel::onCheckBoxToggled);
  return check_box;
}

QLineEdit* SettingsModel::addLineEdit(const QString& property) {
  QLineEdit* line_edit = new QLineEdit();
  line_edit->setObjectName("line_edit");

  // Save property name to widget's property-list.
  line_edit->setProperty(kSettingsPropName, property);

  // Read current property value.
  line_edit->setText(GetSettingsString(property));

  // Watch for changes.
  connect(line_edit, &QLineEdit::textChanged,
          this, &SettingsModel::onLineEditChanged);
  return line_edit;
}

QSlider* SettingsModel::addSlider(const QString& property, int minimum,
                                  int maximum) {
  QSlider* slider = new QSlider();
  slider->setObjectName("slider");

  // Save property name to widget's property-list.
  slider->setProperty(kSettingsPropName, property);

  // Read current property value.
  slider->setRange(minimum, maximum);
  slider->setOrientation(Qt::Horizontal);
  slider->setValue(GetSettingsInt(property));

  // Watch for changes.
  connect(slider, &QSlider::valueChanged,
          this, &SettingsModel::onSliderChanged);

  return slider;
}

QString SettingsModel::defaultLocale() const {
  return GetSettingsString(kSelectLanguageDefaultLocale);
}

void SettingsModel::setDefaultLocale(const QString& locale) {
  SetSettingsValue(kSelectLanguageDefaultLocale, locale);
}

void SettingsModel::onCheckBoxToggled(bool toggled) {
  // Get property name from source widget.
  const QString prop = this->sender()->property(kSettingsPropName).toString();
  SetSettingsValue(prop, toggled);
}

void SettingsModel::onLineEditChanged(const QString& text) {
  const QString prop = this->sender()->property(kSettingsPropName).toString();
  SetSettingsValue(prop, text);
}

void SettingsModel::onSliderChanged(int value) {
  const QString prop = this->sender()->property(kSettingsPropName).toString();
  SetSettingsValue(prop, value);
}

}  // namespace installer