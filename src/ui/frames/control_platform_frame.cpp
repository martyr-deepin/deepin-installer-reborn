#include "control_platform_frame.h"

#include <QComboBox>
#include <QDebug>
#include <QEvent>
#include <QFile>
#include <QVBoxLayout>
#include <QtNetwork>
#include <QLabel>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDBusInterface>

#include "ui/widgets/line_edit.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/table_combo_box.h"
#include "service/settings_manager.h"
#include "ui/utils/widget_util.h"
#include "ui/widgets/title_label.h"
#include "ui/widgets/comment_label.h"
#include "service/settings_name.h"

using namespace installer;

ControlPlatformFrame::ControlPlatformFrame(QWidget* parent)
    : QWidget(parent)
    , m_titleLbl(new TitleLabel(tr("Set Control Region")))
    , m_subTitleLbl(new CommentLabel(tr("Set the region for Deepin EndPoint Management Platform")))
    , m_serverLineEdit(new LineEdit(QString(":/images/hostname_12.svg")))
    , m_nextButton(new NavButton)
    , m_regionInfo(nullptr)
    , m_macInfoLayout(new QVBoxLayout)
    , m_ipInfoLayout(new QVBoxLayout)
{
    QDBusInterface* iface = new QDBusInterface(
        "org.freedesktop.NetworkManager", "/org/freedesktop/NetworkManager",
        "org.freedesktop.NetworkManager", QDBusConnection::systemBus(), this);

    if (iface->isValid()) {
        connect(iface, SIGNAL(StateChanged(uint)), this, SLOT(onNetworkStateChanged()));
    }

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);

    QLabel *logo_label = new QLabel;
    logo_label->setPixmap(installer::renderPixmap(GetVendorLogo()));

    layout->addWidget(logo_label, 0, Qt::AlignHCenter);
    layout->addWidget(m_titleLbl, 0, Qt::AlignHCenter);
    layout->addWidget(m_subTitleLbl, 0, Qt::AlignHCenter);

    m_titleLbl->setObjectName("TitleLabel");
    m_subTitleLbl->setObjectName("SubTitleLabel");

    layout->addStretch();

    m_regionBoxLayout = new QVBoxLayout;
    m_regionBoxLayout->setMargin(0);
    m_regionBoxLayout->setSpacing(10);

    QHBoxLayout* ipInfo = new QHBoxLayout;
    ipInfo->setMargin(0);
    ipInfo->setSpacing(0);

    ipInfo->addStretch();
    ipInfo->addLayout(m_macInfoLayout);
    ipInfo->addSpacing(30);
    ipInfo->addLayout(m_ipInfoLayout);
    ipInfo->addStretch();

    layout->addLayout(ipInfo);
    layout->addSpacing(20);
    layout->addWidget(m_serverLineEdit, 0, Qt::AlignHCenter);
    layout->addLayout(m_regionBoxLayout);
    layout->addStretch();
    layout->addWidget(m_nextButton, 0, Qt::AlignHCenter);

    m_serverLineEdit->setText(GetSettingsString(kEndPointControlServerUrl));
    m_serverLineEdit->setReadOnly(GetSettingsBool(kEndPointControlLockServer));

    QTimer::singleShot(0, m_serverLineEdit, &LineEdit::editingFinished);

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

    setStyleSheet("QLabel{color: white;}");

    onNetworkStateChanged();
}

bool ControlPlatformFrame::event(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        m_nextButton->setText(tr("Next"));
        m_serverLineEdit->setPlaceholderText(tr("Server Address"));
        m_titleLbl->setText(tr("Set Control Region"));
        m_subTitleLbl->setText(tr("Set the region for Deepin EndPoint Management Platform"));
    }

    return QWidget::event(event);
}

void ControlPlatformFrame::onNetworkFinished(QNetworkReply* reply)
{
    QTextCodec*   codec = QTextCodec::codecForName("utf8");
    const QString all   = codec->toUnicode(reply->readAll());
    reply->deleteLater();
    m_regionInfoList.clear();

    QJsonArray array = QJsonDocument::fromJson(all.toUtf8()).array();

    QList<RegionInfo::Ptr> regionList;
    std::map<int, RegionInfo::Ptr> infos;
    std::map<int, QList<RegionInfo::Ptr>> maps;
    for (QJsonValue value : array) {
        RegionInfo::Ptr info(new RegionInfo);
        info->formJson(std::move(value.toObject()));
        infos[info->Id] = info;
        maps[info->parent_id] << info;
        regionList << info;
    }

    if (infos.empty()) {
        qWarning() << "Network Error! lise is empty!";
        return;
    }

    QList<RegionInfo::Ptr> list;
    for (std::pair<int, RegionInfo::Ptr> ptr : infos) {
        for (auto it = maps.begin(); it != maps.end(); ++it) {
            if (it->first == ptr.first) {
                ptr.second->children = it->second;
                list << it->second;
                break;
            }
        }
    }

    for (RegionInfo::Ptr ptr : list) {
        regionList.removeOne(ptr);
    }

    for (TableComboBox* box : m_tableComboBoxList) {
        m_regionBoxLayout->removeWidget(box);
        box->deleteLater();
    }

    m_tableComboBoxList.clear();

    m_tableComboBoxList << createComboBox(regionList);
    m_regionInfoList    << regionList;
}

void ControlPlatformFrame::onNextClicked()
{
    // save config
    QFile file("/etc/dmcg/config.json");
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QJsonObject obj;
        obj["protocol"]    = m_serverUrl.scheme();
        obj["server_port"] = m_serverUrl.port();
        obj["server_host"] = m_serverUrl.host();
        obj["area_id"]     = m_regionInfo->Id;
        obj["Area"] = m_regionInfo->toJson();
        QJsonDocument doc;
        doc.setObject(obj);
        file.write(doc.toJson());
        file.close();
    }

    emit requestFinished();
}

void ControlPlatformFrame::onRegionSelected()
{
    TableComboBox* box = qobject_cast<TableComboBox*>(sender());

    int index = m_tableComboBoxList.indexOf(box);
    auto list = m_tableComboBoxList;

    for (int i = index + 1; i != list.size(); ++i) {
        TableComboBox* b = list[i];
        m_regionBoxLayout->removeWidget(b);
        m_tableComboBoxList.removeOne(b);
        b->deleteLater();
    }

    m_regionInfo = m_regionInfoList[index][box->currentIndex()];

    if (!m_regionInfo->children.isEmpty()) {
        m_tableComboBoxList << createComboBox(m_regionInfo->children);
        m_regionInfoList    << m_regionInfo->children;
    }

    m_nextButton->setEnabled(true);
}

void ControlPlatformFrame::onNetworkStateChanged() {
    auto deleteAllChild = [=](QLayout* layout) -> void {
        QLayoutItem* child;
        while ((child = layout->takeAt(0)) != 0) {
            if (child->widget()) {
                child->widget()->setParent(nullptr);
            }

            delete child;
        }
    };

    deleteAllChild(m_macInfoLayout);
    deleteAllChild(m_ipInfoLayout);

    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface& interface : interfaces) {
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        for (const QNetworkAddressEntry& entry : entryList) {
            const QHostAddress& address = entry.ip();
            if (address == QHostAddress::LocalHost || address == QHostAddress::LocalHostIPv6 || address == QHostAddress::AnyIPv6) {
                continue;
            }
            bool ok = false;
            address.toIPv4Address(&ok);
            if (!ok) {
                continue;
            }
            m_macInfoLayout->addWidget(new QLabel(QString("mac: %1").arg(interface.hardwareAddress())));
            m_ipInfoLayout->addWidget(new QLabel(QString("IP: %1").arg(address.toString())));
        }
    }
}

TableComboBox* ControlPlatformFrame::createComboBox(const QList<RegionInfo::Ptr> &list)
{
    TableComboBox* box = new TableComboBox;
    ControlPlatformRegionModel* model = new ControlPlatformRegionModel(box);
    box->setModel(model);
    model->setList(list);
    m_regionBoxLayout->addWidget(box, 0, Qt::AlignHCenter);
    connect(box,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &ControlPlatformFrame::onRegionSelected);

    return box;
}
