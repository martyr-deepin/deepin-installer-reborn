// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.


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