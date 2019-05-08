#include "networkframe.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>
#include <list>

#include "ui/widgets/line_edit.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/system_info_tip.h"

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
    m_validityCheck = std::unique_ptr<
        QRegularExpressionValidator>(new QRegularExpressionValidator(QRegularExpression(
        "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?\\.){0,3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?")));

    std::list<LineEdit *> editList = { m_ipv4Edit, m_maskEdit, m_primaryDNSEdit,
                                       m_secondDNSEdit };

    for (auto it = editList.begin(); it != editList.end(); ++it) {
        (*it)->setValidator(m_validityCheck.get());
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);

    layout->addStretch();

    for (auto it = editList.begin(); it != editList.end(); ++it) {
        layout->addWidget((*it), 0, Qt::AlignHCenter);
    }

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
