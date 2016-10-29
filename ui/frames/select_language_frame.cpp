// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/select_language_frame.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QPalette>
#include <QPixmap>
#include <QScrollArea>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/delegates/language_list_delegate.h"
#include "ui/models/language_list_model.h"
#include "ui/widgets/frosted_frame.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/subhead_label.h"

namespace installer {

SelectLanguageFrame::SelectLanguageFrame(QWidget* parent)
    : QFrame(parent),
      locale_() {
  this->setObjectName(QStringLiteral("select_language_frame"));

  this->initUI();
  this->initConnections();
  // TODO(xushaohua): Load default locale.
}

void SelectLanguageFrame::autoConf() {
  locale_ = GetSettingsString(kSelectLanguageDefaultLocale);
  emit this->languageSelected(locale_);
  WriteLocale(locale_);
}

void SelectLanguageFrame::initConnections() {
  connect(next_button_, &QPushButton::clicked,
          this, &SelectLanguageFrame::onNextButtonClicked);
  connect(list_view_, &QListView::clicked,
          this, &SelectLanguageFrame::onLanguageListSelected);
}

void SelectLanguageFrame::initUI() {
  QLabel* logo_label = new QLabel();
  logo_label->setPixmap(QPixmap(GetVendorLogo()));

  SubheadLabel* subhead_label = new SubheadLabel(
      "Please select the language you want to use\n请选择您的语言");

  list_view_ = new QListView();
  QSizePolicy list_policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  list_policy.setHorizontalStretch(1);
  list_policy.setVerticalStretch(1);
  list_view_->setSizePolicy(list_policy);
  list_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QPalette list_palette = list_view_->palette();
  list_palette.setBrush(QPalette::Base, Qt::transparent);
  list_view_->setPalette(list_palette);
  list_view_->setSelectionMode(QAbstractItemView::SingleSelection);
  list_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  list_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  list_model_ = new LanguageListModel(this);
  list_view_->setModel(list_model_);
  list_view_->setFixedWidth(468);
  list_view_->setSelectionRectVisible(false);
  list_view_->setUniformItemSizes(true);
//  LanguageListDelegate* list_delegate = new LanguageListDelegate(this);
//  list_view_->setItemDelegate(list_delegate);
  list_view_->setStyleSheet(ReadTextFileContent(":/styles/language_list.css"));

  next_button_ = new NavButton(tr("Next"));
  next_button_->setEnabled(false);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(logo_label, 0, Qt::AlignCenter);
  layout->addWidget(subhead_label, 0, Qt::AlignCenter);
  layout->addSpacing(20);
  layout->addWidget(list_view_, 0, Qt::AlignHCenter);
  layout->addSpacing(20);
  layout->addWidget(next_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

void SelectLanguageFrame::onLanguageListSelected() {
  next_button_->setEnabled(true);
  const QModelIndexList selected_items =
      list_view_->selectionModel()->selectedIndexes();
  if (selected_items.length() == 1) {
    locale_ = list_model_->locale(selected_items.at(0));
    emit this->languageSelected(locale_);
  }
}

void SelectLanguageFrame::onNextButtonClicked() {
  WriteLocale(locale_);
  emit this->finished();
}

}  // namespace installer