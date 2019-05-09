#ifndef NETWORKFRAME_H
#define NETWORKFRAME_H

#include <QWidget>
#include <memory>

class QRegularExpressionValidator;
class QLabel;
namespace installer {
class LineEdit;
class NavButton;
class SystemInfoTip;
class NetworkFrame : public QWidget {
    Q_OBJECT
public:
    explicit NetworkFrame(QWidget* parent = nullptr);

signals:
    void requestNext();

protected:
    bool event(QEvent *event) override;

private:
    void saveConf();
    void checkIPValidity();
    void checkMaskValidity();

private:
    QLabel*                                      m_subTitle;
    LineEdit*                                    m_ipv4Edit;
    LineEdit*                                    m_maskEdit;
    LineEdit*                                    m_gatewayEdit;
    LineEdit*                                    m_primaryDNSEdit;
    LineEdit*                                    m_secondDNSEdit;
    NavButton*                                   m_skipButton;
    NavButton*                                   m_saveButton;
    std::unique_ptr<QRegularExpressionValidator> m_validityCheck;
    SystemInfoTip*                               m_errorTip;
};
}  // namespace installer

#endif  // NETWORKFRAME_H
