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

#include "ui/widgets/simple_disk_button.h"

#include <QLabel>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/delegates/partition_util.h"
#include "ui/utils/widget_util.h"

namespace installer {

namespace {

const int kButtonWidth = 220;
const int kButtonHeight = 220;
const int kOsIconWidth = 120;
const int kOsIconHeight = 120;

const QString kDriverIcon = ":/images/driver_128.svg";
const QString kDriverInstallIcon = ":/images/driver_install_128.svg";

}  // namespace

SimpleDiskButton::SimpleDiskButton(const Device::Ptr device, QWidget* parent)
  : PointerButton(parent),
    device_(device),
    selected_(false) {
  this->setObjectName("simple_disk_button");
  this->setFixedSize(kButtonWidth, kButtonHeight);
  this->setCheckable(true);
  this->initUI();
}

void SimpleDiskButton::setSelected(bool selected) {
    selected_ = selected;
    os_label_->setPixmap(installer::renderPixmap(selected ? kDriverInstallIcon : kDriverIcon));
}

void SimpleDiskButton::initUI() {
  os_label_ = new QLabel();
  os_label_->setObjectName("fs_label");
  os_label_->setFixedSize(kOsIconWidth, kOsIconHeight);
  os_label_->setPixmap(installer::renderPixmap(kDriverIcon));

  QLabel* path_label = new QLabel();
  path_label->setObjectName("path_label");
  path_label->setText(device_->path);

  QLabel* model_label = new QLabel();
  model_label->setObjectName("model_label");
  model_label->setText(device_->model);

  QLabel* size_label = new QLabel();
  size_label->setObjectName("size_label");
  size_label->setText(QString("%1 GB").arg(ToGigByte(device_->getByteLength())));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addStretch();
  layout->addWidget(os_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(6);
  layout->addWidget(path_label, 0, Qt::AlignHCenter);
  layout->addSpacing(6);
  layout->addWidget(model_label, 0, Qt::AlignHCenter);
  layout->addSpacing(6);
  layout->addWidget(size_label, 0, Qt::AlignHCenter);
  layout->addStretch();

  this->setLayout(layout);

  this->setStyleSheet(ReadFile(":/styles/simple_disk_button.css"));
  this->setCheckable(true);
  this->setFixedSize(kButtonWidth, kButtonHeight);
}

}  // namespace installer
