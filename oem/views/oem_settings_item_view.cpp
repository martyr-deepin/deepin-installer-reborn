// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "oem/views/oem_settings_item_view.h"

#include <QGridLayout>
#include <QLabel>

namespace installer {

OemSettingsItemView::OemSettingsItemView(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName("oem_settings_item_view");
  this->initUI();
  this->initConnections();
}

void OemSettingsItemView::updateItem(const OemSettingsItem& item) {
  name_->setText(item.name());
  desc_->setText(item.description());
  type_->setText(OemSettingsTypeToString(item.value_type()));
  default_value_->setText(item.default_value().toString());
  value_->setText(item.value().toString());
}

void OemSettingsItemView::initConnections() {

}

void OemSettingsItemView::initUI() {
  QLabel* name_label = new QLabel(tr("Name"));
  name_ = new QLabel();
  QLabel* desc_label = new QLabel(tr("Description"));
  desc_ = new QLabel();
  QLabel* type_label = new QLabel(tr("Type"));
  type_ = new QLabel();
  QLabel* default_value_label = new QLabel(tr("Default value"));
  default_value_ = new QLabel();
  QLabel* value_label = new QLabel(tr("Value"));
  value_ = new QLabel();

  QGridLayout* layout = new QGridLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(name_label, 0, 0);
  layout->addWidget(name_, 0, 1);
  layout->addWidget(desc_label, 1, 0);
  layout->addWidget(desc_, 1, 1);
  layout->addWidget(type_label, 2, 0);
  layout->addWidget(type_, 2, 1);
  layout->addWidget(default_value_label, 3, 0);
  layout->addWidget(default_value_, 3, 1);
  layout->addWidget(value_label, 4, 0);
  layout->addWidget(value_, 4, 1);

  this->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);
}

}  // namespace installer