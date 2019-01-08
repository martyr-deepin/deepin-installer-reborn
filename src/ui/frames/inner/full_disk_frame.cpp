/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#include "ui/frames/inner/full_disk_frame.h"

#include <QButtonGroup>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QCheckBox>
#include <QApplication>

#include "base/file_util.h"
#include "partman/device.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/full_disk_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/utils/widget_util.h"
#include "ui/widgets/simple_disk_button.h"

namespace installer {

namespace {

// 4 partitions are displays at each row.
const int kDiskColumns = 4;

const int kWindowWidth = 960;

}  // namespace

#if 0
QT_TRANSLATE_NOOP("FullDiskFrame", "Install here")
QT_TRANSLATE_NOOP("FullDiskFrame", "Encrypt Full Disk")
QT_TRANSLATE_NOOP("FullDiskFrame", "Please select a disk to start installation")
#endif

FullDiskFrame::FullDiskFrame(FullDiskDelegate* delegate, QWidget* parent)
    : QFrame(parent)
    , m_delegate(delegate)
{
  this->setObjectName("simple_disk_frame");

  this->initUI();
  this->initConnections();

  m_delegate->saveSwapSize();
}

FullDiskFrame::~FullDiskFrame() {

}

bool FullDiskFrame::validate() const {
    bool isSelect = false;
    for (QAbstractButton* button : m_button_group->buttons()) {
        if (button->isChecked()) {
            isSelect = true;
            break;
        }
    }

    if (!isSelect) {
        m_errorTip->show();
        return false;
    }

    return (m_button_group->checkedButton() != nullptr);
}

bool FullDiskFrame::isEncrypt() const
{
    return m_encryptCheck->isChecked();
}

void FullDiskFrame::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        for (auto it = m_trList.begin(); it != m_trList.end(); ++it) {
            it->first(qApp->translate("installer::FullDiskFrame", it->second.toUtf8()));
        }
    }
    else {
        QFrame::changeEvent(event);
    }
}

void FullDiskFrame::initConnections() {
  // Repaint layout only when real device list is updated.
  connect(m_delegate, &FullDiskDelegate::deviceRefreshed,
          this, &FullDiskFrame::onDeviceRefreshed);
  connect(m_button_group,
          static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>
          (&QButtonGroup::buttonToggled),
          this, &FullDiskFrame::onPartitionButtonToggled);
  connect(m_encryptCheck, &QCheckBox::clicked, this, &FullDiskFrame::cryptoStateChanged);
}

void FullDiskFrame::initUI() {
  m_button_group = new QButtonGroup(this);

  QLabel* tip_icon = new QLabel();
  tip_icon->setPixmap(installer::renderPixmap(":/images/install_icon.svg"));
  m_tip_label = new QLabel(tr("Install here"));
  m_tip_label->setObjectName("tip_label");
  m_tip_label->setFixedHeight(18);
  addTransLate(m_trList, std::bind(&QLabel::setText, m_tip_label, std::placeholders::_1), QString("Install here"));

  m_encryptCheck = new QCheckBox;
  m_encryptCheck->setObjectName("check_box");
  m_encryptCheck->setCheckable(true);
  m_encryptCheck->setChecked(false);
  m_encryptCheck->setFocusPolicy(Qt::NoFocus);
  addTransLate(m_trList, std::bind(&QCheckBox::setText, m_encryptCheck, std::placeholders::_1), QString("Encrypt Full Disk"));

  m_errorTip = new QLabel;
  m_errorTip->setObjectName("msg_label");
  m_errorTip->hide();
  addTransLate(m_trList, std::bind(&QLabel::setText, m_errorTip, std::placeholders::_1), QString("Please select a disk to start installation"));

  QHBoxLayout* tip_layout = new QHBoxLayout();
  tip_layout->setContentsMargins(0, 0, 0, 0);
  tip_layout->setSpacing(0);
  tip_layout->addStretch();
  tip_layout->addWidget(tip_icon, 0, Qt::AlignVCenter);
  tip_layout->addWidget(m_tip_label, 0, Qt::AlignVCenter);
  tip_layout->addStretch();

  m_install_tip = new QFrame();
  m_install_tip->setObjectName("install_tip");
  m_install_tip->setContentsMargins(0, 0, 0, 0);
  // Same width as SimplePartitionButton.
  m_install_tip->setFixedWidth(220);
  m_install_tip->setLayout(tip_layout);
  m_install_tip->hide();

  m_grid_layout = new QGridLayout();
  m_grid_layout->setSpacing(0);
  m_grid_layout->setContentsMargins(0, 0, 0, 0);
  m_grid_layout->setHorizontalSpacing(20);
  m_grid_layout->setVerticalSpacing(20);
  m_grid_layout->setColumnStretch(kDiskColumns, 1);

  m_grid_wrapper = new QFrame();
  m_grid_wrapper->setFixedWidth(kWindowWidth);
  m_grid_wrapper->setObjectName("grid_wrapper");
  m_grid_wrapper->setLayout(m_grid_layout);
  m_install_tip->setParent(m_grid_wrapper);

  QScrollArea* scroll_area = new QScrollArea();
  scroll_area->setObjectName("scroll_area");
  scroll_area->setWidget(m_grid_wrapper);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area->setWidgetResizable(true);
  scroll_area->setFixedWidth(kWindowWidth);

  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->addWidget(scroll_area, 0, Qt::AlignHCenter);
  main_layout->addStretch();
  main_layout->addWidget(m_encryptCheck, 0, Qt::AlignHCenter);
  main_layout->addSpacing(20);
  main_layout->addWidget(m_errorTip, 0, Qt::AlignHCenter);

  m_encryptCheck->setVisible(!GetSettingsBool(KPartitionSkipFullCryptPage));

  this->setLayout(main_layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setFixedWidth(kWindowWidth);
  QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  policy.setVerticalStretch(100);
  this->setSizePolicy(policy);
  this->setStyleSheet(ReadFile(":/styles/simple_disk_frame.css"));

  for (auto it = m_trList.begin(); it != m_trList.end(); ++it) {
      it->first(qApp->translate("installer::FullDiskFrame", it->second.toUtf8()));
  }
}

void FullDiskFrame::repaintDevices() {
  // Clear grid layout.
  ClearLayout(m_grid_layout);

  // Clear button group.
  for (QAbstractButton* button : m_button_group->buttons()) {
    m_button_group->removeButton(button);
  }

  // Hide tooltip frame
  m_install_tip->hide();

  // Draw partitions.
  int row = 0, column = 0;
  for (const Device::Ptr device : m_delegate->virtual_devices()) {
    bool partition_busy = false;
    for (const Partition::Ptr partition : device->partitions) {
      if (partition->busy) {
        partition_busy = true;
        break;
      }
    }
    if (partition_busy) {
      qWarning() << "Partition on device is busy!" << device->path;
      continue;
    }
    SimpleDiskButton* button = new SimpleDiskButton(device);

    m_button_group->addButton(button);
    m_grid_layout->addWidget(button, row, column, Qt::AlignHCenter);

    column += 1;
    // Add rows.
    if (column >= kDiskColumns) {
      column = 0;
      row += 1 ;
    }
  }

  // Add place holder. It is used for install_tip
  row += 1;
  QLabel* place_holder_label = new QLabel(this);
  place_holder_label->setObjectName("place_holder_label");
  place_holder_label->setFixedSize(kWindowWidth, 30);
  m_grid_layout->addWidget(place_holder_label, row, 0,
                          1, kDiskColumns, Qt::AlignHCenter);

  // Resize grid_wrapper explicitly.
  m_grid_wrapper->adjustSize();
}

void FullDiskFrame::showInstallTip(QAbstractButton* button) {
  // Move install_tip to bottom of button
  const QPoint pos = button->pos();
  m_install_tip->move(pos.x(), pos.y() - 10);
  m_install_tip->show();
}

void FullDiskFrame::onDeviceRefreshed() {
  this->repaintDevices();
}

void FullDiskFrame::onPartitionButtonToggled(QAbstractButton* button,
                                             bool checked) {
  SimpleDiskButton* part_button = dynamic_cast<SimpleDiskButton*>(button);
  if (!part_button) {
    qCritical() << "no disk button is selected";
    return;
  }

  m_errorTip->hide();

  if (!checked) {
    // Deselect previous button.
    part_button->setSelected(false);
  } else {
    // Hide tooltip.
    m_install_tip->hide();

    emit currentDeviceChanged(part_button->device());

    const QString path = part_button->device()->path;
    qDebug() << "selected device path:" << path;
    part_button->setSelected(true);

    // Show install-tip at bottom of current checked button.
    this->showInstallTip(part_button);

    // Reset simple operations.
    m_delegate->resetOperations();

    WriteFullDiskDeivce(path);


//    PartitionTableType table = IsEfiEnabled() ?
//                               PartitionTableType::GPT :
//                               PartitionTableType::MsDos;
//    m_delegate->formatWholeDevice(path, table);
  }
}

}  // namespace installer
