// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_BOOTLOADER_LIST_MODEL_H
#define INSTALLER_UI_MODELS_BOOTLOADER_LIST_MODEL_H

#include <QAbstractListModel>
#include <QStringList>

namespace installer {

class PartitionDelegate;

// Reimplemented model used in SelectBootloaderFrame
class BootloaderListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  BootloaderListModel(PartitionDelegate* delegate, QObject* parent = nullptr);

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

 private:
  PartitionDelegate* delegate_ = nullptr;

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

 private slots:
  // Update bootloader list when device list is updated in delegate.
  void onDeviceRefreshed();
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_BOOTLOADER_LIST_MODEL_H
