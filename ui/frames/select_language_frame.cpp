// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/select_language_frame.h"

#include <QLabel>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/models/language_list_model.h"
#include "ui/views/frameless_list_view.h"
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
  // TODO(xushaohua): Select the appropriate locale.
}

void SelectLanguageFrame::autoConf() {
  locale_ = GetSettingsString(kSelectLanguageDefaultLocale);
  emit this->languageSelected(locale_);
  WriteLocale(locale_);
}

void SelectLanguageFrame::initConnections() {
  connect(next_button_, &QPushButton::clicked,
          this, &SelectLanguageFrame::onNextButtonClicked);
  connect(language_view_, &FramelessListView::clicked,
          this, &SelectLanguageFrame::onLanguageListSelected);
}

void SelectLanguageFrame::initUI() {
  QLabel* logo_label = new QLabel();
  logo_label->setPixmap(QPixmap(GetVendorLogo()));

  SubheadLabel* subhead_label = new SubheadLabel(
      "Please select the language you want to use\n请选择您的语言");

  language_view_ = new FramelessListView();
  language_model_ = new LanguageListModel(language_view_);
  language_view_->setModel(language_model_);

  next_button_ = new NavButton(tr("Next"));
  next_button_->setEnabled(false);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(logo_label, 0, Qt::AlignCenter);
  layout->addWidget(subhead_label, 0, Qt::AlignCenter);
  layout->addSpacing(20);
  layout->addWidget(language_view_, 0, Qt::AlignHCenter);
  layout->addSpacing(20);
  layout->addWidget(next_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

void SelectLanguageFrame::onLanguageListSelected() {
  next_button_->setEnabled(true);
  const QModelIndexList selected_items =
      language_view_->selectionModel()->selectedIndexes();
  if (selected_items.length() == 1) {
    locale_ = language_model_->locale(selected_items.at(0));
    emit this->languageSelected(locale_);
  }
}

void SelectLanguageFrame::onNextButtonClicked() {
  WriteLocale(locale_);
  emit this->finished();
}

}  // namespace installer