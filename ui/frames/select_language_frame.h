/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef INSTALLER_UI_FRAMES_SELECT_LANGUAGE_FRAME_H
#define INSTALLER_UI_FRAMES_SELECT_LANGUAGE_FRAME_H

#include <QFrame>
#include <QModelIndex>
class QTranslator;

#include "service/system_language.h"

namespace installer {

class FramelessListView;
class LanguageListModel;
class NavButton;

// Select system language and update ui language on-the-fly.
class SelectLanguageFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SelectLanguageFrame(QWidget* parent = nullptr);

  // Read default locale.
  void readConf();

  // Write locale to settings file.
  void writeConf();

 signals:
  void finished();

  // Emitted when new language item is selected.
  void timezoneUpdated(const QString& timezone);

 protected:
  // Update text of next_button_
  void changeEvent(QEvent* event) override;

  // Handles key press event of language_view_.
  bool eventFilter(QObject* obj, QEvent* event);

 private:
  void initConnections();
  void initUI();
  void updateTranslator(const QString& locale);

  // Current selected language.
  LanguageItem lang_;

  QTranslator* current_translator_ = nullptr;

  FramelessListView* language_view_ = nullptr;
  LanguageListModel* language_model_ = nullptr;

  NavButton* next_button_ = nullptr;

 private slots:
  void onLanguageListSelected(const QModelIndex& current,
                              const QModelIndex& previous);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_SELECT_LANGUAGE_FRAME_H
