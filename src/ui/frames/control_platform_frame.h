#ifndef CONTROLPLATFORMFRAME_H
#define CONTROLPLATFORMFRAME_H

#include <QWidget>
#include <QUrl>

#include "ui/models/control_platform_region_model.h"

class QComboBox;
class QNetworkReply;
namespace installer {
class LineEdit;
class NavButton;
class ControlPlatformRegionModel;
class TableComboBox;
class ControlPlatformFrame : public QWidget {
    Q_OBJECT
public:
    ControlPlatformFrame(QWidget* parent = nullptr);

signals:
    void requestFinished();

protected:
    bool event(QEvent* event) override;

private:
    void onNetworkFinished(QNetworkReply* reply);
    void onNextClicked();
    void onRegionSelected();

private:
    LineEdit*                   m_serverLineEdit;
    TableComboBox*              m_regionBox;
    NavButton*                  m_nextButton;
    ControlPlatformRegionModel* m_regionModel;
    QUrl                        m_serverUrl;
    QList<RegionInfo>           m_regionInfo;
};
}  // namespace installer
#endif  // !CONTROLPLATFORMFRAME_H
