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

#include "ui/widgets/line_edit.h"

#include <QFocusEvent>
#include <QLabel>
#include <QResizeEvent>

#include "base/file_util.h"
#include "ui/utils/widget_util.h"

namespace installer {

LineEdit::LineEdit(const QString& icon, QWidget* parent) : QLineEdit(parent)
{
    this->setObjectName("line_edit");

    this->setStyleSheet(ReadFile(":/styles/line_edit.css"));
    // Same width as in table combobox.
    this->setFixedSize(310, 36);

    // Disable context menu.
    this->setContextMenuPolicy(Qt::NoContextMenu);

    image_label_ = new QLabel(this);
    image_label_->setPixmap(installer::renderPixmap(icon));
    image_label_->setFixedSize(12, 12);
    image_label_->move(12, 12);

    m_right_label = new QLabel(this);
    m_right_label->setFixedSize(12, 12);
    m_right_label->move(310 - 12 - 10, (36 - 12) / 2);
    m_right_label->setPixmap(installer::renderPixmap(":/images/capslock.svg"));
    m_right_label->hide();
}

void LineEdit::setCapsLockVisible(bool visible)
{
    m_right_label->setVisible(visible);
}

void LineEdit::focusInEvent(QFocusEvent* event)
{
    QLineEdit::focusInEvent(event);
    emit this->gotFocus();
}

void LineEdit::resizeEvent(QResizeEvent* event)
{
    image_label_->move(12, 12);
    QLineEdit::resizeEvent(event);
}

}  // namespace installer
