// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "oem/oem_window.h"

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QSplitter>

#include "base/file_util.h"
#include "oem/models/oem_settings_model.h"
#include "oem/views/oem_settings_item_view.h"

namespace installer {

OemWindow::OemWindow(QWidget* parent) : QFrame(parent) {
  this->setObjectName("oem_window");

  this->initUI();
  this->initConnections();

  // TODO(xushaohua): catch exceptions.
  if (!model_->load()) {
    qCritical() << "model->load() failed";
  }
}

void OemWindow::initConnections() {
  connect(name_list_view_->selectionModel(),
          &QItemSelectionModel::currentChanged,
          this, &OemWindow::onNameListViewSelected);
  connect(item_view_, &OemSettingsItemView::itemChanged,
          model_, &OemSettingsModel::updateItem);

  // Dump items before program exits.
  connect(qApp, &QApplication::aboutToQuit,
          model_, &OemSettingsModel::dumpItems);
}

void OemWindow::initUI() {
  model_ = new OemSettingsModel(this);
  name_list_view_ = new QListView(this);
  name_list_view_->setModel(model_);

  item_view_ = new OemSettingsItemView();

  QSplitter* splitter = new QSplitter(this);
  splitter->setContentsMargins(0, 0, 0, 0);
  splitter->setOpaqueResize(false);
  splitter->addWidget(name_list_view_);
  splitter->addWidget(item_view_);
  // Update stretch factor so that item_view_ can be resized dynamically.
  splitter->setStretchFactor(0, 0);
  splitter->setStretchFactor(1, 1);

  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(splitter);

  this->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);
}

void OemWindow::onNameListViewSelected(const QModelIndex& current,
                                       const QModelIndex& previous) {
  Q_UNUSED(previous);
  const OemSettingsItem item = model_->getItem(current);
  item_view_->updateItem(item);
}

}  // namespace installer