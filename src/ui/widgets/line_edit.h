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

#ifndef INSTALLER_UI_WIDGETS_LINE_EDIT_H
#define INSTALLER_UI_WIDGETS_LINE_EDIT_H

#include <QLineEdit>
class QFocusEvent;
class QLabel;
class QResizeEvent;

namespace installer {

// Customized line edit used in form page.
class LineEdit : public QLineEdit {
    Q_OBJECT

public:
    LineEdit(const QString& icon, QWidget* parent = nullptr);
    void setCapsLockVisible(bool visible);

signals:
    void gotFocus();

protected:
    // Emit gotFocus() signal when line-edit get focus
    void focusInEvent(QFocusEvent* event) override;

    // Reset image label position.
    void resizeEvent(QResizeEvent* event) override;

private:
    QLabel* image_label_  = nullptr;
    QLabel* m_right_label = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_LINE_EDIT_H
