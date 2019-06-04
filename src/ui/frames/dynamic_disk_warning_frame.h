#ifndef DYNAMIC_DISK_WARNING_FRAME_H
#define DYNAMIC_DISK_WARNING_FRAME_H

#include <QLabel>
#include <QWidget>
#include <utility>
#include "partman/device.h"

class QHBoxLayout;

namespace installer {
class NavButton;
class DynamicDiskWarningFrame : public QWidget {
    Q_OBJECT
public:
    explicit DynamicDiskWarningFrame(QWidget* parent = nullptr);
    void setDevice(const QList<Device::Ptr> list);
    void setWarningTip(const QString& tip);

signals:
    void requestCancel();
    void requestNext();

public slots:

protected:
    bool event(QEvent* event) override;

private:
    void refreshTs();

private:
    QLabel*      m_warning;
    QLabel*      m_warningTips;
    NavButton*   m_cancelBtn;
    NavButton*   m_acceptBtn;
    QHBoxLayout* m_diskListLayout;
};
}  // namespace installer

#endif  // !DYNAMIC_DISK_WARNING_FRAME_H
