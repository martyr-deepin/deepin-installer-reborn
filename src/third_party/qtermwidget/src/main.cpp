/*  Copyright (C) 2008 e_k (e_k@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/


#include <QApplication>
#include <QtDebug>

#include "qtermwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTermWidget *console = new QTermWidget();

    QFont font = QApplication::font();
    font.setFamily("Monospace");
    font.setPointSize(14);
    console->setTerminalFont(font);

    console->setColorScheme("WhiteOnBlack");
    console->setScrollBarPosition(QTermWidget::ScrollBarRight);

    for (const QString& arg : QApplication::arguments()) {
        if (console->availableColorSchemes().contains(arg)) {
            console->setColorScheme(arg);
        }
        if (console->availableKeyBindings().contains(arg)) {
            console->setKeyBindings(arg);
        }
    }

    // info output
    qDebug() << "* INFO *************************";
    qDebug() << " availableKeyBindings:" << console->availableKeyBindings();
    qDebug() << " keyBindings:" << console->keyBindings();
    qDebug() << " availableColorSchemes:" << console->availableColorSchemes();
    qDebug() << "* INFO END *********************";

    // real startup
    QObject::connect(console, SIGNAL(finished()), &app, SLOT(quit()));

    console->resize(640, 480);
    console->show();
    return app.exec();
}
