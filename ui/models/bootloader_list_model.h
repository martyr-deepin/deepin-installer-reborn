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

#ifndef INSTALLER_UI_MODELS_BOOTLOADER_LIST_MODEL_H
#define INSTALLER_UI_MODELS_BOOTLOADER_LIST_MODEL_H

#include <QAbstractListModel>
#include <QStringList>

#include "partman/device.h"

namespace installer {

// Reimplemented model used in SelectBootloaderFrame
class BootloaderListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit BootloaderListModel(QObject* parent = nullptr);

  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual int rowCount(const QModelIndex& parent) const override;

  // Get partition path at |row|.
  QString getPath(const QModelIndex index) const;
  QModelIndex getRecommendedIndex() const;

 signals:
  // Emitted when row data in list is changed.
  // This signal is read in SelectBootloaderFrame. It is used to avoid race
  // condition.
  void rowChanged();

 public slots:
  // Update bootloader list when device list is updated in delegate.
  void onDeviceRefreshed(const DeviceList& devices);

 private:
  struct PartitionItem {
    PartitionItem(const QString& path, const QString& label, bool recommended)
        : path(path), label(label), recommended(recommended) {}

    QString path;
    QString label;

    // Recommended device/partition path used for bootloader.
    // Normally it is "/dev/sda".
    bool recommended = false;
  };

  // To store path of available partitions and devices.
  QList<PartitionItem> bootloader_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_BOOTLOADER_LIST_MODEL_H
