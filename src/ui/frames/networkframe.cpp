#include "networkframe.h"

#include <QVBoxLayout>

#include "ui/widgets/line_edit.h"
#include "ui/widgets/nav_button.h"

using namespace installer;

NetworkFrame::NetworkFrame(QWidget *parent)
    : QWidget(parent)
    , m_ipv4Edit(new LineEdit(QString(":/images/hostname_12.svg")))
    , m_maskEdit(new LineEdit(QString(":/images/hostname_12.svg")))
    , m_primaryDNSEdit(new LineEdit(QString(":/images/hostname_12.svg")))
    , m_secondDNSEdit(new LineEdit(QString(":/images/hostname_12.svg")))
    , m_skipButton(new NavButton(tr("Skip")))
    , m_saveButton(new NavButton(tr("Next")))
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);

    layout->addStretch();
    layout->addWidget(m_ipv4Edit, 0, Qt::AlignHCenter);
    layout->addWidget(m_maskEdit, 0, Qt::AlignHCenter);
    layout->addWidget(m_primaryDNSEdit, 0, Qt::AlignHCenter);
    layout->addWidget(m_secondDNSEdit, 0, Qt::AlignHCenter);
    layout->addSpacing(10);
    layout->addWidget(m_skipButton, 0, Qt::AlignHCenter);
    layout->addWidget(m_saveButton, 0, Qt::AlignHCenter);
    layout->addStretch();

    setLayout(layout);

    connect(m_saveButton, &NavButton::clicked, this, &NetworkFrame::saveConf);
}

void NetworkFrame::saveConf()
{
    emit requestNext();
}
