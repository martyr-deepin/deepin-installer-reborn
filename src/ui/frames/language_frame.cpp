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

#include "ui/frames/language_frame.h"
#include "ui/frames/inner/select_language_frame.h"
#include "ui/frames/inner/user_agreement_frame.h"

namespace installer {
LanguageFrame::LanguageFrame(QWidget *parent)
    : QWidget(parent)
    , m_frame_layout(new QStackedLayout)
    , m_select_language_frame(new SelectLanguageFrame)
    , m_user_license_frame(new UserAgreementFrame)
{
    initUI();
    initConnect();
}

LanguageFrame::~LanguageFrame() {}

void LanguageFrame::readConf() {
    m_select_language_frame->readConf();
}

void LanguageFrame::writeConf() {
    m_select_language_frame->writeConf();
}

void LanguageFrame::initUI() {
    m_frame_layout->setMargin(0);
    m_frame_layout->addWidget(m_select_language_frame);
    m_frame_layout->addWidget(m_user_license_frame);

    setLayout(m_frame_layout);
}

void LanguageFrame::initConnect() {
    connect(m_select_language_frame, &SelectLanguageFrame::finished, this,
            &LanguageFrame::finished);
    connect(m_select_language_frame, &SelectLanguageFrame::timezoneUpdated, this,
            &LanguageFrame::timezoneUpdated);
    connect(m_select_language_frame, &SelectLanguageFrame::requestShowUserLicense, this,
            &LanguageFrame::showUserLicense);
    connect(m_user_license_frame, &UserAgreementFrame::back, this,
            &LanguageFrame::showLanguage);
}

void LanguageFrame::showUserLicense() {
    m_frame_layout->setCurrentWidget(m_user_license_frame);
}

void LanguageFrame::showLanguage() {
    m_frame_layout->setCurrentWidget(m_select_language_frame);
}

}  // namespace installer
