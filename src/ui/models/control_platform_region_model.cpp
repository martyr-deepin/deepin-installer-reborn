#include "control_platform_region_model.h"

#include <QStringList>

using namespace installer;

ControlPlatformRegionModel::ControlPlatformRegionModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void ControlPlatformRegionModel::setList(const QList<RegionInfo> &list) {
    m_list = list;

    emit layoutChanged();
}

const RegionInfo ControlPlatformRegionModel::findInfoByIndex(int index) const {
    return m_list.value(index, RegionInfo());
}

QVariant ControlPlatformRegionModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    const int row = index.row();

    return m_list[row].name;
}

int ControlPlatformRegionModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_list.size();
}
