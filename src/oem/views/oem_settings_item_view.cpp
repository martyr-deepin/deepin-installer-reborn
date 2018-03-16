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

#include "oem/views/oem_settings_item_view.h"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStringListModel>
#include <QTextEdit>

#include "base/file_util.h"

namespace installer {

OemSettingsItemView::OemSettingsItemView(QWidget* parent)
    : QFrame(parent),
      item_() {
  this->setObjectName("oem_settings_item_view");
  this->initUI();
  this->initConnections();
}

void OemSettingsItemView::updateItem(const OemSettingsItem& item) {
  // Display content of |item|.
  item_ = item;
  title_->setText(item.title());
  name_->setText(item.name());
  desc_->setText(item.description());
  type_->setText(OemSettingsTypeToString(item.value_type()));
  const QString default_value = item.default_value().toString();
  const QString value = item.value().toString();
  default_value_->setText(default_value);
  const bool use_default_value = (default_value == value);
  this->updateCurrentValue();

  use_default_value_btn_->blockSignals(true);
  use_default_value_btn_->setChecked(use_default_value);
  use_default_value_btn_->blockSignals(false);

  this->enableCustomValue(!use_default_value);
  this->updateCustomValue();
}

void OemSettingsItemView::initConnections() {
  connect(use_default_value_btn_, &QPushButton::toggled,
          this, &OemSettingsItemView::onUseDefaultValueButtonToggled);

  connect(custom_bool_, &QPushButton::toggled,
          this, &OemSettingsItemView::onCustomWidgetValueChanged);
  connect(custom_combo_,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &OemSettingsItemView::onCustomWidgetValueChanged);
  connect(custom_spin_box_,
          static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this, &OemSettingsItemView::onCustomWidgetValueChanged);
  connect(custom_line_edit_, &QLineEdit::editingFinished,
          this, &OemSettingsItemView::onCustomWidgetValueChanged);
  connect(custom_text_edit_, &QTextEdit::textChanged,
          this, &OemSettingsItemView::onCustomWidgetValueChanged);
}

void OemSettingsItemView::initUI() {
  QLabel* title_label = new QLabel(tr("Title"));
  title_ = new QLabel();
  QLabel* name_label = new QLabel(tr("Name"));
  name_ = new QLabel();
  QLabel* desc_label = new QLabel(tr("Description"));
  desc_ = new QLabel();
  desc_->setWordWrap(true);
  QLabel* type_label = new QLabel(tr("Type"));
  type_ = new QLabel();
  QLabel* default_value_label = new QLabel(tr("Default value"));
  default_value_ = new QLabel();
  QLabel* value_label = new QLabel(tr("Current value"));
  value_ = new QLabel();
  QLabel* use_default_value_label = new QLabel(tr("Use default value"));
  use_default_value_btn_ = new QPushButton();
  use_default_value_btn_->setCheckable(true);
  QLabel* custom_value_label = new QLabel(tr("Custom value"));
  custom_bool_ = new QPushButton();
  custom_bool_->setCheckable(true);
  custom_combo_ = new QComboBox();
  custom_list_model_ = new QStringListModel(this);
  custom_combo_->setModel(custom_list_model_);
  custom_line_edit_ = new QLineEdit();
  custom_spin_box_ = new QSpinBox();
  custom_text_edit_ = new QTextEdit();

  QVBoxLayout* custom_layout = new QVBoxLayout();
  custom_layout->setContentsMargins(0, 0, 0, 0);
  custom_layout->setSpacing(0);
  custom_layout->addWidget(custom_bool_, Qt::AlignLeft);
  custom_layout->addWidget(custom_combo_, Qt::AlignLeft);
  custom_layout->addWidget(custom_line_edit_, Qt::AlignLeft);
  custom_layout->addWidget(custom_spin_box_, Qt::AlignLeft);
  custom_layout->addWidget(custom_text_edit_, Qt::AlignLeft);

  QGridLayout* grid = new QGridLayout();
  grid->setContentsMargins(0, 0, 0, 0);
  grid->setSpacing(0);
  grid->setHorizontalSpacing(15);
  grid->setVerticalSpacing(20);
  // Make value column resizable.
  grid->setColumnStretch(0, 0);
  grid->setColumnStretch(1, 1);
  grid->addWidget(title_label, 0, 0, Qt::AlignRight | Qt::AlignTop);
  grid->addWidget(title_, 0, 1);
  grid->addWidget(name_label, 1, 0, Qt::AlignRight | Qt::AlignTop);
  grid->addWidget(name_, 1, 1);
  grid->addWidget(desc_label, 2, 0, Qt::AlignRight | Qt::AlignTop);
  grid->addWidget(desc_, 2, 1);
  grid->addWidget(type_label, 3, 0, Qt::AlignRight | Qt::AlignTop);
  grid->addWidget(type_, 3, 1, Qt::AlignLeft);
  grid->addWidget(default_value_label, 4, 0, Qt::AlignRight | Qt::AlignTop);
  grid->addWidget(default_value_, 4, 1);
  grid->addWidget(value_label, 5, 0, Qt::AlignRight | Qt::AlignTop);
  grid->addWidget(value_, 5, 1);
  grid->addWidget(use_default_value_label, 6, 0, Qt::AlignRight | Qt::AlignTop);
  grid->addWidget(use_default_value_btn_, 6, 1, Qt::AlignLeft);
  grid->addWidget(custom_value_label, 7, 0, Qt::AlignRight | Qt::AlignTop);
  grid->addLayout(custom_layout, 7, 1, Qt::AlignLeft);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addLayout(grid);
  layout->addStretch();

  this->setContentsMargins(10, 15, 5, 5);
  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/oem_settings_item_view.css"));
}

void OemSettingsItemView::enableCustomValue(bool enable) {
  // Update custom value area.
  custom_bool_->setVisible(false);
  custom_combo_->setVisible(false);
  custom_line_edit_->setVisible(false);
  custom_spin_box_->setVisible(false);
  custom_text_edit_->setVisible(false);

  switch (item_.value_type()) {
    case OemSettingsType::Base64String: {
      custom_text_edit_->setVisible(true);
      custom_text_edit_->setEnabled(enable);
      break;
    }
    case OemSettingsType::Boolean: {
      custom_bool_->setVisible(true);
      custom_bool_->setEnabled(enable);
      break;
    }
    case OemSettingsType::Enumeration: {
      custom_combo_->setVisible(true);
      custom_combo_->setEnabled(enable);
      break;
    }
    case OemSettingsType::Integer: {
      custom_spin_box_->setVisible(true);
      custom_spin_box_->setEnabled(enable);
      break;
    }
    case OemSettingsType::StringArray: {
      custom_text_edit_->setVisible(true);
      custom_text_edit_->setEnabled(enable);
      break;
    }
    case OemSettingsType::String: {
      custom_line_edit_->setVisible(true);
      custom_line_edit_->setEnabled(enable);
      break;
    }
    default: {
    }
  }
}

QVariant OemSettingsItemView::getCustomValue() {
  switch (item_.value_type()) {
    case OemSettingsType::Base64String: {
      return custom_text_edit_->document()->toPlainText();
    }
    case OemSettingsType::Boolean: {
      return custom_bool_->isChecked();
    }
    case OemSettingsType::Enumeration: {
      return custom_combo_->currentText();
    }
    case OemSettingsType::Integer: {
      return custom_spin_box_->value();
    }
    case OemSettingsType::StringArray: {
      // TODO(xushaohua): merge string array.
      const QString content = custom_text_edit_->document()->toPlainText();
      return content;
    }
    case OemSettingsType::String: {
      return custom_line_edit_->text();
    }
    default: {
      // Returns an empty string if this type is not supported.
      return "";
    }
  }
}

void OemSettingsItemView::updateCurrentValue() {
  const QString default_value = item_.default_value().toString();
  const QString value = item_.value().toString();
  if (default_value == value) {
    value_->setText(tr("Default value"));
  } else {
    if (item_.value_type() == OemSettingsType::Base64String) {
      // Read decoded string.
      value_->setText(value);
    } else {
      value_->setText(value);
    }
  }
}

void OemSettingsItemView::updateCustomValue() {
  switch (item_.value_type()) {
    case OemSettingsType::Base64String: {
      const QString content = item_.value().toString();
      custom_text_edit_->blockSignals(true);
      custom_text_edit_->setText(content);
      custom_text_edit_->blockSignals(false);
      break;
    }
    case OemSettingsType::Boolean: {
      custom_bool_->blockSignals(true);
      custom_bool_->setChecked(item_.value().toBool());
      custom_bool_->blockSignals(false);
      break;
    }
    case OemSettingsType::Enumeration: {
      custom_combo_->blockSignals(true);
      custom_list_model_->setStringList(item_.options());
      const QString locale = item_.value().toString();
      const int index = item_.options().indexOf(locale);
      if (index > -1) {
        custom_combo_->setCurrentIndex(index);
      }
      custom_combo_->blockSignals(false);
      break;
    }
    case OemSettingsType::Integer: {
      custom_spin_box_->blockSignals(true);
      custom_spin_box_->setRange(item_.minimum(), item_.maximum());
      custom_spin_box_->setValue(item_.value().toInt());
      custom_spin_box_->blockSignals(false);
      break;
    }
    case OemSettingsType::StringArray: {
      custom_text_edit_->blockSignals(true);
      custom_text_edit_->setText(item_.value().toString());
      custom_text_edit_->blockSignals(false);
      break;
    }
    case OemSettingsType::String: {
      custom_line_edit_->blockSignals(true);
      custom_line_edit_->setText(item_.value().toString());
      custom_line_edit_->blockSignals(false);
      break;
    }
    default: {
    }
  }
}

void OemSettingsItemView::onUseDefaultValueButtonToggled(bool checked) {
  this->enableCustomValue(!checked);
  if (checked) {
    // Restore to default value.
    item_.setValue(item_.default_value());
    this->updateCurrentValue();
  } else {
    // Update its value to custom value.
    item_.setValue(this->getCustomValue());
    this->updateCurrentValue();
  }

  emit this->itemChanged(item_);
}

void OemSettingsItemView::onCustomWidgetValueChanged() {
  if (!use_default_value_btn_->isChecked()) {
    item_.setValue(this->getCustomValue());

    // Update text of value_ label.
    this->updateCurrentValue();

    // And emit itemChanged() signal.
    emit this->itemChanged(item_);
  }
}

}  // namespace installer