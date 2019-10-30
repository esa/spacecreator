#include "propsfiltermodel.h"

#include "propertieslistmodel.h"

namespace taste3 {
namespace aadl {

PropsFilterModel::PropsFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

PropertiesListModel::ItemType PropsFilterModel::targetType() const
{
    return m_targetType;
}

void PropsFilterModel::setTargetType(PropertiesListModel::ItemType type)
{
    if (type != m_targetType) {
        m_targetType = type;
        invalidateFilter();
    }
}

bool PropsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    return index.data(PropertiesListModel::ItemTypeRole).toInt() == targetType();
}

} // namespace aadl
} // namespace taste3
