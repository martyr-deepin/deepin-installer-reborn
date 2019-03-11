/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     justforlxz <zhangdingyuan@deepin.com>
 *
 * Maintainer: justforlxz <zhangdingyuan@deepin.com>
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

#ifndef LANGUAGEFRAME_H
#define LANGUAGEFRAME_H

#include <QWidget>
#include <QStackedLayout>

namespace installer {
class SelectLanguageFrame;
class UserAgreementFrame;
class LanguageFrame : public QWidget {
    Q_OBJECT
public:
    LanguageFrame(QWidget *parent = nullptr);
    ~LanguageFrame();

    // Read default locale.
    void readConf();

    // Write locale to settings file.
    void writeConf();

signals:
    void finished();

    // Emitted when new language item is selected.
    void timezoneUpdated(const QString& timezone);

private:
    void initUI();
    void initConnect();
    void showUserLicense();
    void showLanguage();

private:
    QStackedLayout*      m_frame_layout          = nullptr;
    SelectLanguageFrame* m_select_language_frame = nullptr;
    UserAgreementFrame*  m_user_license_frame    = nullptr;
};

}  // namespace installer

#endif  // !LANGUAGEFRAME_H
