#include "actionsmodel.h"

#include <QDebug>

namespace taste3 {
namespace aadl {

ActionsModel::ActionsModel(QObject *parent)
    : QStringListModel(parent)
{
}

void ActionsModel::setupData(QVector<ctx::Action> *collection)
{
    beginResetModel();

    if (m_collection != collection)
        m_collection = collection;

    endResetModel();
}

int ActionsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_collection ? m_collection->size() : 0;
}

QVariant ActionsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: {
        const int row = index.row();
        if (row >= 0 && row < m_collection->size())
            return m_collection->at(row).m_title;
        break;
    }
    default:
        break;
    }

    return QStringListModel::data(index, role);
}

bool ActionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {

        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole: {
            const int row = index.row();
            if (row >= 0 && row < m_collection->size()) {
                m_collection->operator[](row).m_title = value.toString();
                emit dataChanged(index, index, QVector<int>() << role);
                return true;
            }
            break;
        }
        default:
            break;
        }
    }
    return QStringListModel::setData(index, value, role);
}

Qt::ItemFlags ActionsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QStringListModel::flags(index) | Qt::ItemIsEditable;
}

bool ActionsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        ctx::Action action;
        action.m_conditions.append(ctx::Condition::createGlobal());
        m_collection->insert(row + i, action);
    }
    endInsertRows();
    return true;
}

bool ActionsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_collection->removeAt(row + i);
    endRemoveRows();
    return true;
}

QModelIndex ActionsModel::createAction(const QString &name, const QString &target)
{
    const int r = rowCount();
    insertRow(r);
    QModelIndex id = index(r);
    setData(id, name);
    m_collection->operator[](r).m_internalActName = target;
    return id;
}

} // namespace aadl
} // namespace taste3
