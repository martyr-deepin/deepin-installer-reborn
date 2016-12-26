// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_PARTITION_TYPE_MODEL_H
#define INSTALLER_UI_MODELS_PARTITION_TYPE_MODEL_H

#include <QAbstractListModel>

namespace installer {

// Model to display available partition type in NewPartitionFrame.
class PartitionTypeModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit PartitionTypeModel(QObject* parent = nullptr);

  QVariant data(const QModelIndex& index, int role) const override;
  int rowCount(const QModelIndex& parent) const override;

  // Get index of logical partition.
  int getLogicalIndex() const;

  // Get index of primary partition.
  int getPrimaryIndex() const;

  // Check whether selected item at |index| is logical partition.
  bool isLogical(int index) const;

  // Check whether selected item at |index| is primary partition.
  bool isPrimary(int index) const;

 public slots:
  // Reset status. Both "primary partition" and "logical partition" are
  // available now.
  void reset();

  // Set visibility of "logical partition" option.
  void setLogicalVisible(bool visible);

  // Set visibility of "primary partition" option.
  void setPrimaryVisible(bool visible);

 private:
  bool is_logical_visible_;
  bool is_primary_visible_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_PARTITION_TYPE_MODEL_H
