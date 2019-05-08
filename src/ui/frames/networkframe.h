#ifndef NETWORKFRAME_H
#define NETWORKFRAME_H

#include <QWidget>

namespace installer {
class LineEdit;
class NavButton;
class NetworkFrame : public QWidget {
    Q_OBJECT
public:
    explicit NetworkFrame(QWidget* parent = nullptr);

signals:
    void requestNext();

private:
    void saveConf();

private:
    LineEdit*  m_ipv4Edit;
    LineEdit*  m_maskEdit;
    LineEdit*  m_primaryDNSEdit;
    LineEdit*  m_secondDNSEdit;
    NavButton* m_skipButton;
    NavButton* m_saveButton;
};
}  // namespace installer

#endif  // NETWORKFRAME_H
