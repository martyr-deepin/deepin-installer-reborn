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

#include "ui/frames/inner/system_info_keyboard_frame.h"

#include <QDebug>
#include <QEvent>
#include <QLineEdit>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/models/keyboard_layout_model.h"
#include "ui/models/keyboard_layout_variant_model.h"
#include "ui/utils/widget_util.h"
#include "ui/views/frameless_list_view.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const int kLayoutWidth = 860;

}  // namespace

SystemInfoKeyboardFrame::SystemInfoKeyboardFrame(QWidget* parent)
    : QFrame(parent),
      current_locale_() {
  this->setObjectName("system_info_keyboard_frame");

  this->initUI();
  this->initConnections();
}

void SystemInfoKeyboardFrame::readConf() {
  // Load xkb config first.
  current_locale_ = ReadLocale();
  layout_model_->initLayout(current_locale_);

  const QString layout = GetSettingsString(kSystemInfoDefaultKeyboardLayout);
  if (layout.isEmpty()) {
    qWarning() << "Default keyboard layout is empty!";
    return;
  }

  const QModelIndex index = layout_model_->getLayoutByName(layout);
  if (index.isValid()) {
    // Select default layout.
    layout_view_->setCurrentIndex(index);
  } else {
    qWarning() << "Invalid default keyboard layout:" << layout;
  }
}

void SystemInfoKeyboardFrame::writeConf() {
  const QModelIndex layout_index = layout_view_->currentIndex();
  const QString layout = layout_model_->getLayoutName(layout_index);
  const QModelIndex variant_index = variant_view_->currentIndex();
  const QString variant = variant_model_->getVariantName(variant_index);

  // Model name of keyboard is empty. Variant name might be empty.
  // The first row in variant list is the default layout.
  if (variant_index.row() == 0) {
    WriteKeyboard("", layout, "");
  } else {
    WriteKeyboard("", layout, variant);
  }
}

void SystemInfoKeyboardFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Select keyboard layout"));
    test_edit_->setPlaceholderText(tr("Test area"));
    back_button_->setText(tr("Back"));
  } else {
    QFrame::changeEvent(event);
  }
}

void SystemInfoKeyboardFrame::initConnections() {
  connect(layout_view_->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &SystemInfoKeyboardFrame::onLayoutViewSelectionChanged);
  connect(variant_view_->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &SystemInfoKeyboardFrame::onVariantViewSelected);

  connect(back_button_, &QPushButton::clicked,
          this, &SystemInfoKeyboardFrame::finished);
}

void SystemInfoKeyboardFrame::initUI() {
  title_label_ = new TitleLabel(tr("Select keyboard layout"));

  layout_view_ = new FramelessListView();
  layout_view_->setObjectName("layout_view");
  layout_model_ = new KeyboardLayoutModel(this);
  layout_view_->setModel(layout_model_);
  layout_view_->setFixedWidth(340);

  variant_view_ = new FramelessListView();
  variant_view_->setObjectName("variant_view");
  variant_view_->setFixedWidth(519);
  variant_model_ = new KeyboardLayoutVariantModel(this);
  variant_view_->setModel(variant_model_);

  QHBoxLayout* keyboard_layout = new QHBoxLayout();
  keyboard_layout->setContentsMargins(0, 0, 0, 0);
  keyboard_layout->setSpacing(0);
  keyboard_layout->addStretch();
  keyboard_layout->addWidget(layout_view_);
  // Add 1px margin between these two list views.
  keyboard_layout->addSpacing(1);
  keyboard_layout->addWidget(variant_view_);
  keyboard_layout->addStretch();

  QFrame* keyboard_wrapper = new QFrame();
  keyboard_wrapper->setObjectName("keyboard_wrapper");
  keyboard_wrapper->setFixedWidth(kLayoutWidth);
  keyboard_wrapper->setContentsMargins(0, 0, 0, 0);
  keyboard_wrapper->setLayout(keyboard_layout);
  QSizePolicy keyboard_size_policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  keyboard_size_policy.setVerticalStretch(1);
  keyboard_wrapper->setSizePolicy(keyboard_size_policy);

  test_edit_ = new QLineEdit();
  test_edit_->setObjectName("test_edit");
  test_edit_->setPlaceholderText(tr("Test area"));
  test_edit_->setFixedSize(kLayoutWidth, 36);
  // Disable context menu.
  test_edit_->setContextMenuPolicy(Qt::NoContextMenu);

  back_button_ = new NavButton(tr("Back"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addSpacing(kMainLayoutSpacing + 40);
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addSpacing(kMainLayoutSpacing + 30);
  layout->addWidget(keyboard_wrapper, 0, Qt::AlignHCenter);
  layout->addSpacing(10);
  layout->addWidget(test_edit_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addSpacing(kMainLayoutSpacing + 30);
  layout->addWidget(back_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  const QString style = ReadFile(":/styles/system_info_keyboard_frame.css");
  this->setStyleSheet(style);

  // Update style of list views, adding border radius.
  AppendStyleSheet(layout_view_, style);
  AppendStyleSheet(variant_view_, style);
}

void SystemInfoKeyboardFrame::onLayoutViewSelectionChanged(
    const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);
  test_edit_->clear();

  // Update variant list.
  variant_model_->setVariantList(layout_model_->getVariantList(current),
                                 current_locale_);

  // Scroll to top of variant view.
  variant_view_->scrollToTop();

  // Select the default layout variant.
  variant_view_->setCurrentIndex(variant_model_->index(0));
}

void SystemInfoKeyboardFrame::onVariantViewSelected(
    const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);

  // Clear content of test_edit_ when new layout variant is selected.
  test_edit_->clear();

  const QModelIndex layout_index = layout_view_->currentIndex();
  const QString layout = layout_model_->getLayoutName(layout_index);
  const QString variant = variant_model_->getVariantName(current);
  QString description;

  // The first row in variant list is the default layout.
  if (current.row() == 0) {
    description = layout_model_->getLayoutDescription(layout_index);
    if (!SetXkbLayout(layout)) {
      qWarning() << "SetXkbLayout() failed!" << layout;
    }
  } else {
    description = variant_model_->getVariantDescription(current);
    if (!SetXkbLayout(layout, variant)) {
      qWarning() << "SetXkbLayout() failed!" << layout << variant;
    }
  }
  emit this->layoutUpdated(description);
}

}  // namespace installer