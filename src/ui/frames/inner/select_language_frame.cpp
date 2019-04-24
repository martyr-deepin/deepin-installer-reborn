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

#include "ui/frames/inner/select_language_frame.h"

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QLabel>
#include <QTranslator>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QScrollBar>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/language_delegate.h"
#include "ui/frames/consts.h"
#include "ui/models/language_list_model.h"
#include "ui/views/frameless_list_view.h"
#include "ui/widgets/nav_button.h"
#include "ui/utils/widget_util.h"

namespace installer {

SelectLanguageFrame::SelectLanguageFrame(QWidget* parent)
    : QFrame(parent),
      lang_(),
      current_translator_(new QTranslator(this)) {
  this->setObjectName("select_language_frame");

  this->initUI();
  this->initConnections();

  language_view_->installEventFilter(this);
}

void SelectLanguageFrame::readConf() {
// Select default locale
  const QString locale = GetSettingsString(kSelectLanguageDefaultLocale);
  const QModelIndex index = language_model_->localeIndex(locale);
  if (index.isValid()) {
    lang_ = language_model_->languageItemAt(index);
    language_view_->setCurrentIndex(index);
    emit language_view_->clicked(index);
  }
}

void SelectLanguageFrame::writeConf() {
  WriteLocale(lang_.locale);
}

void SelectLanguageFrame::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        return updateTs();
    }
    else {
        QFrame::changeEvent(event);
    }
}

bool SelectLanguageFrame::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress && next_button_->isEnabled()) {
        QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
        if (key_event->key() == Qt::Key_Return || key_event->key() == Qt::Key_Enter) {
            // Simulate button click event.
            next_button_->click();
            return true;
        }
    }

    if (obj == license_label_) {
        switch (event->type()) {
            case QEvent::MouseButtonRelease: emit requestShowUserLicense(); break;
            case QEvent::Enter: setCursor(QCursor(Qt::PointingHandCursor)); break;
            case QEvent::Leave: setCursor(QCursor(Qt::ArrowCursor)); break;
            default: break;
        }
    }

    return QObject::eventFilter(obj, event);
}

void SelectLanguageFrame::initConnections() {
  connect(language_view_, &QListView::clicked, this,
          &SelectLanguageFrame::onLanguageListSelected);
  connect(
      language_view_->selectionModel(), &QItemSelectionModel::currentChanged, this,
      [=](const QModelIndex& current, const QModelIndex& previous) {
          // Skip first signal
          if (current == language_view_->model()->index(0, 0) && !previous.isValid()) {
              return;
          }
          Q_UNUSED(previous);
          emit language_view_->clicked(current);
      });
  connect(next_button_, &QPushButton::clicked,
          this, &SelectLanguageFrame::finished);
  connect(accept_license_, &QCheckBox::clicked, this,
          &SelectLanguageFrame::onAccpetLicenseChanged);
}

void SelectLanguageFrame::initUI() {
  QLabel* logo_label = new QLabel();
  logo_label->setPixmap(installer::renderPixmap(GetVendorLogo()));

  QLabel* subtitle_label = new QLabel("Select system language");
  subtitle_label->setObjectName("subtitle_label");
  subtitle_label->setWordWrap(false);
  subtitle_label->setAlignment(Qt::AlignHCenter);
  QLabel* subtitle2_label = new QLabel("请选择您的语言");
  subtitle2_label->setObjectName("subtitle2_label");
  subtitle2_label->setWordWrap(false);
  subtitle2_label->setAlignment(Qt::AlignHCenter);

  language_view_ = new FramelessListView();
  language_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  language_view_->setContextMenuPolicy(Qt::NoContextMenu);
  language_view_->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
  language_view_->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
  language_model_ = new LanguageListModel(language_view_);
  language_view_->setModel(language_model_);

  accept_license_ = new QCheckBox;
  accept_license_->setCheckable(true);
  accept_license_->setChecked(false);
  accept_license_->setFocusPolicy(Qt::NoFocus);

  license_label_ = new QLabel;
  license_label_->setObjectName("LicenseLabel");
  license_label_->installEventFilter(this);

  QHBoxLayout *license_layout = new QHBoxLayout;
  license_layout->setMargin(0);
  license_layout->setSpacing(5);
  license_layout->addStretch();
  license_layout->addWidget(accept_license_);
  license_layout->addWidget(license_label_);
  license_layout->addStretch();

  next_button_ = new NavButton(tr("Next"));
  next_button_->setEnabled(false);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addSpacing(30);
  layout->addWidget(logo_label, 0, Qt::AlignCenter);
  layout->addWidget(subtitle_label, 0, Qt::AlignCenter);
  layout->addWidget(subtitle2_label, 0, Qt::AlignCenter);
  layout->addSpacing(20);
  layout->addWidget(language_view_, 0, Qt::AlignHCenter);
  layout->addSpacing(20);
  layout->addLayout(license_layout);
  layout->addSpacing(20);
  layout->addWidget(next_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setStyleSheet(ReadFile(":/styles/select_language_frame.css"));

  updateTs();
}

void SelectLanguageFrame::updateTranslator(const QString& locale) {
  if (!lang_.locale.isEmpty()) {
    // Remove the old translator if it is loaded.
    qApp->removeTranslator(current_translator_);
  }
  const QString locale_file(GetLocalePath(locale));
  if (current_translator_->load(locale_file)) {
    if (!qApp->installTranslator(current_translator_)) {
      qWarning() << "Failed to update ui language at:" << locale_file;
    }
  } else {
    qWarning() << "Failed to load locale file:" << locale_file;
  }
}

void SelectLanguageFrame::updateTs() {
  next_button_->setText(tr("Next"));
  accept_license_->setText(tr("I have read and agree to"));
  license_label_->setText(tr("Deepin Software End User License Agreement"));
}

void SelectLanguageFrame::onLanguageListSelected(const QModelIndex& current)
{
    if (current.isValid()) {
        // Update locale on-the-fly.
        const LanguageItem language_item = language_model_->languageItemAt(current);
        this->updateTranslator(language_item.locale);
        lang_ = language_item;
        this->writeConf();
        emit this->timezoneUpdated(language_item.timezone);
        onAccpetLicenseChanged(accept_license_->isChecked());
    }
}

void SelectLanguageFrame::onAccpetLicenseChanged(bool enable) {
    next_button_->setEnabled(enable && !lang_.name.isEmpty());
}

}  // namespace installer
