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


#include <QApplication>
#include <QDebug>
#include <QFrame>
#include <QLineEdit>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "third_party/zlib_wrapper/zip_util.h"
#include "ui/widgets/qr_widget.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QByteArray content;
  installer::ReadRawFile("/tmp/content.txt", content);
  const QByteArray b64_dec(QByteArray::fromBase64(content));
  const QByteArray raw_content = qUncompress(b64_dec);
  qDebug() << raw_content;

  QFrame window;

  QLineEdit* edit = new QLineEdit();
  installer::QRWidget* qr_widget = new installer::QRWidget();
  qr_widget->setFixedSize(300, 300);
  QObject::connect(edit, &QLineEdit::textChanged,
                   [&](const QString& text) {
                     const QByteArray raw_data = text.toUtf8();
                     QByteArray compressed_data;
                     if (!GzipCompress(raw_data, compressed_data, 9)) {
                       qCritical() << "gzip failed";
                       return;
                     }
                     const QByteArray b64_data = compressed_data.toBase64();
                     const QString output = QString::fromUtf8(b64_data);
                     qDebug() << text.length() << "=>" << output.length();
                     qr_widget->setText(output);
                   });

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(edit);
  layout->addWidget(qr_widget);

  window.setLayout(layout);
  window.show();

  return app.exec();
}