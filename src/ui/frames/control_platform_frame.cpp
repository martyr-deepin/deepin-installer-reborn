#include "control_platform_frame.h"

#include <QComboBox>
#include <QDebug>
#include <QEvent>
#include <QFile>
#include <QVBoxLayout>
#include <QtNetwork>

#include "ui/widgets/line_edit.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/table_combo_box.h"

using namespace installer;

ControlPlatformFrame::ControlPlatformFrame(QWidget* parent)
    : QWidget(parent)
    , m_serverLineEdit(new LineEdit(QString(":/images/hostname_12.svg")))
    , m_regionBox(new TableComboBox)
    , m_nextButton(new NavButton)
    , m_regionModel(new ControlPlatformRegionModel(this))
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);

    m_regionBox->setModel(m_regionModel);

    layout->addStretch();
    layout->addWidget(m_serverLineEdit, 0, Qt::AlignHCenter);
    layout->addWidget(m_regionBox, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(m_nextButton, 0, Qt::AlignHCenter);

    m_nextButton->setEnabled(false);

    setLayout(layout);

    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);

    connect(networkAccessManager, &QNetworkAccessManager::finished, this,
            &ControlPlatformFrame::onNetworkFinished);

    connect(m_serverLineEdit, &LineEdit::editingFinished, this, [=] {
        const QString& value = m_serverLineEdit->text();
        if (value.isEmpty()) return;

        m_serverUrl = QUrl(QString("%1/api/all_area").arg(value));
        networkAccessManager->get(QNetworkRequest(m_serverUrl));
    });

    connect(m_nextButton, &NavButton::clicked, this,
            &ControlPlatformFrame::onNextClicked);
    connect(m_regionBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &ControlPlatformFrame::onRegionSelected);
}

bool ControlPlatformFrame::event(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        m_nextButton->setText(tr("Next"));
        m_serverLineEdit->setPlaceholderText(tr("Server Address"));
    }

    return QWidget::event(event);
}

void ControlPlatformFrame::onNetworkFinished(QNetworkReply* reply)
{
    QTextCodec*   codec = QTextCodec::codecForName("utf8");
    const QString all   = codec->toUnicode(reply->readAll());
    reply->deleteLater();

    QJsonArray array = QJsonDocument::fromJson(all.toUtf8()).array();

    QList<RegionInfo> list;
    for (const QJsonValue& value : array) {
        RegionInfo info;
        info.formJson(value.toObject());
        list << info;
    }

    m_regionModel->setList(list);
}

void ControlPlatformFrame::onNextClicked()
{
    // save config
    RegionInfo info = m_regionModel->findInfoByIndex(m_regionBox->currentIndex());

    QFile file("/etc/dmcg/config.json");
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QJsonObject obj;
        obj["protocol"]    = m_serverUrl.scheme();
        obj["server_port"] = m_serverUrl.port();
        obj["server_host"] = m_serverUrl.host();
        obj["area_id"]     = info.Id;
        obj["Area"] = info.toJson();
        QJsonDocument doc;
        doc.setObject(obj);
        file.write(doc.toJson());
        file.close();
    }

    emit requestFinished();
}

void ControlPlatformFrame::onRegionSelected()
{
    m_nextButton->setEnabled(true);
}
