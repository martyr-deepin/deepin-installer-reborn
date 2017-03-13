// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "oem/views/oem_settings_item_view.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace installer {

namespace {

const int kValueMaximumWidth = 420;

}  // namespace

OemSettingsItemView::OemSettingsItemView(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName("oem_settings_item_view");
  this->initUI();
  this->initConnections();
}

void OemSettingsItemView::updateItem(const OemSettingsItem& item) {
  title_->setText(item.title());
  name_->setText(item.name());
  desc_->setText(item.description());
  type_->setText(OemSettingsTypeToString(item.value_type()));
  const QString default_value = item.default_value().toString();
  const QString value = item.value().toString();
  default_value_->setText(default_value);
  if (default_value == value) {
    value_->setText(tr("Default value"));
    use_default_value_btn_->setChecked(true);
    custom_value_->setEnabled(false);
  } else {
    value_->setText(value);
    use_default_value_btn_->setChecked(false);
    custom_value_->setEnabled(true);
  }
}

void OemSettingsItemView::initConnections() {

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
  QLabel* default_value_label = new QLabel(tr("value"));
  default_value_ = new QLabel();
  QLabel* value_label = new QLabel(tr("Current value"));
  value_ = new QLabel();
  QLabel* use_default_value_label = new QLabel(tr("Use default value"));
  use_default_value_btn_ = new QPushButton();
  use_default_value_btn_->setCheckable(true);
  QLabel* custom_value_label = new QLabel(tr("Custom value"));
  custom_value_ = new QLabel();

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
  grid->addWidget(custom_value_, 7, 1);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addLayout(grid);
  layout->addStretch();

  this->setContentsMargins(10, 15, 5, 5);
  this->setLayout(layout);
}

}  // namespace installer