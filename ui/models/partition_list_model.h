// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_PARTITION_LIST_MODEL_H
#define INSTALLER_UI_MODELS_PARTITION_LIST_MODEL_H

#include <QAbstractListModel>
#include <QStringList>

namespace installer {

class PartitionDelegate;

// Reimplemented model used in SelectBootloaderFrame
class PartitionListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  PartitionListModel(PartitionDelegate* delegate, QObject* parent = nullptr);

  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual int rowCount(const QModelIndex& parent) const override;

  void updatePartitionList();

 private:
  PartitionDelegate* delegate_ = nullptr;

  // To store path of available partitions and devices.
  QStringList partition_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_PARTITION_LIST_MODEL_H
