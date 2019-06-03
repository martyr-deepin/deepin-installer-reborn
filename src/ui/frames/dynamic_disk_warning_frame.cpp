#include "dynamic_disk_warning_frame.h"
#include "ui/utils/widget_util.h"
#include "ui/widgets/nav_button.h"
#include "base/file_util.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace installer;

DynamicDiskWarningFrame::DynamicDiskWarningFrame(QWidget* parent)
    : QWidget(parent)
    , m_warning(new QLabel)
    , m_warningTips(new QLabel)
    , m_cancelBtn(new NavButton)
    , m_acceptBtn(new NavButton)
    , m_diskListLayout(new QHBoxLayout)
{
    m_warning->setObjectName("WarningText");
    m_warningTips->setObjectName("WarningTips");
    m_warningTips->setWordWrap(true);
    m_warningTips->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_warningTips->setFixedWidth(310);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);

    layout->addStretch();

    m_diskListLayout->setMargin(0);
    m_diskListLayout->setSpacing(30);

    layout->addLayout(m_diskListLayout);

    QHBoxLayout* warningLayout = new QHBoxLayout;
    warningLayout->setMargin(0);
    warningLayout->setSpacing(10);

    QLabel* warningIcon = new QLabel;
    warningIcon->setPixmap(installer::renderPixmap(":/images/warning.svg"));
    warningLayout->addStretch();
    warningLayout->addWidget(warningIcon, 0, Qt::AlignVCenter);
    warningLayout->addWidget(m_warning, 0, Qt::AlignVCenter);
    warningLayout->addStretch();

    layout->addLayout(warningLayout);

    layout->addSpacing(10);
    layout->addWidget(m_warningTips, 1, Qt::AlignHCenter);

    layout->addStretch();
    layout->addWidget(m_cancelBtn, 0, Qt::AlignHCenter);
    layout->addWidget(m_acceptBtn, 0, Qt::AlignHCenter);

    setLayout(layout);

    connect(m_cancelBtn, &NavButton::clicked, this, &DynamicDiskWarningFrame::requestCancel);
    connect(m_acceptBtn, &NavButton::clicked, this, &DynamicDiskWarningFrame::requestNext);

    setStyleSheet(ReadFile(":/styles/dynamic_disk_warning_frame.css"));
}

void DynamicDiskWarningFrame::setDevice(const QList<Device::Ptr> list)
{
    QLayoutItem* child;
    while ((child = m_diskListLayout->takeAt(0)) != 0) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);
        }

        delete child;
    }

    for (const Device::Ptr device : list) {
        QVBoxLayout* diskLayout = new QVBoxLayout;
        diskLayout->setMargin(0);
        diskLayout->setSpacing(0);

        QLabel* diskLbl = new QLabel;
        diskLbl->setPixmap(installer::renderPixmap(":/images/driver_128.svg"));

        QLabel* diskInfoLbl = new QLabel;
        diskInfoLbl->setObjectName("DiskInfoLabel");
        diskInfoLbl->setText(device->path);

        diskLayout->addWidget(diskLbl, 0, Qt::AlignHCenter);
        diskLayout->addSpacing(10);
        diskLayout->addWidget(diskInfoLbl, 0, Qt::AlignHCenter);
        diskLayout->addSpacing(30);

        m_diskListLayout->addLayout(diskLayout);
    }
}

void DynamicDiskWarningFrame::setWarningTip(const QString& tip)
{
    m_warningTips->setText(tip);
}

bool DynamicDiskWarningFrame::event(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        refreshTs();
    }

    return QWidget::event(event);
}

void DynamicDiskWarningFrame::refreshTs()
{
    m_warning->setText(tr("Warning"));
    m_cancelBtn->setText(tr("Cancel"));
    m_acceptBtn->setText(tr("Next"));
}
