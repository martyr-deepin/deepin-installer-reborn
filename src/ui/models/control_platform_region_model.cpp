#include "control_platform_region_model.h"

#include <QStringList>
#include <QDebug>

using namespace installer;

QDebug &operator<<(QDebug &debug, const RegionInfo &info)
{
    debug << info.toJson();
    return debug;
}

QDebug& operator<<(QDebug& debug, const RegionInfo::Ptr info) {
    debug << *info.get();
    return debug;
}

ControlPlatformRegionModel::ControlPlatformRegionModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void ControlPlatformRegionModel::setList(const QList<RegionInfo::Ptr> &list) {
    m_list = list;

    emit layoutChanged();
}

RegionInfo::Ptr ControlPlatformRegionModel::findInfoByIndex(int index) const {
    return m_list.value(index, nullptr);
}

QVariant ControlPlatformRegionModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    const int row = index.row();

    return m_list[row]->name;
}

int ControlPlatformRegionModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_list.size();
}
