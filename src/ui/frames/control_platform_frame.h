#ifndef CONTROLPLATFORMFRAME_H
#define CONTROLPLATFORMFRAME_H

#include <QWidget>

class QComboBox;
namespace installer {
class LineEdit;
class NavButton;
class ControlPlatformFrame : public QWidget {
    Q_OBJECT
public:
    ControlPlatformFrame(QWidget* parent = nullptr);

private:
    LineEdit*  m_serverLineEdit;
    QComboBox* m_regionBox;
    NavButton* m_nextButton;
};
}  // namespace installer
#endif  // !CONTROLPLATFORMFRAME_H
