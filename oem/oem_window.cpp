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

#include "oem/oem_window.h"

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
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
}

bool OemWindow::init() {
  if (!model_->createOemFolders()) {
    // Show warning dialog.
    QMessageBox box;
    box.setText(tr("Failed to create $HOME/oem folder or sub-folders"));
    box.exec();
    return false;
  }

  if (!model_->load()) {
    // Show warning dialog.
    QMessageBox box;
    box.setText(tr("Failed to load model data"));
    box.exec();
    return false;
  }

  return true;
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