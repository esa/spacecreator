/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "conditionsmodel.h"

#include "context/action/condition.h"

namespace taste3 {
namespace ctx {

/*!
 * \class taste3::aadl::ConditionsModel
 * \brief The model to store scriptable actions conditions in editor dialog.
 */
ConditionsModel::ConditionsModel(QObject *parent)
    : QStringListModel(parent)
    , m_conditions(nullptr)
{
}

void ConditionsModel::setupData(QVector<ctx::Condition> *conditions)
{
    beginResetModel();

    if (m_conditions != conditions)
        m_conditions = conditions;

    endResetModel();
}

int ConditionsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_conditions ? m_conditions->size() : 0;
}

QVariant ConditionsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: {
        const int row = index.row();
        if (row >= 0 && row < m_conditions->size())
            return m_conditions->at(row).m_itemType;
        break;
    }
    default:
        break;
    }

    return QStringListModel::data(index, role);
}

bool ConditionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {

        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole: {
            const int row = index.row();
            if (row >= 0 && row < m_conditions->size()) {
                m_conditions->operator[](row).m_itemType = value.toString();
                Q_EMIT dataChanged(index, index, QVector<int>() << role);
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

Qt::ItemFlags ConditionsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QStringListModel::flags(index) & (~Qt::ItemIsEditable);
}

bool ConditionsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        ctx::Condition c;
        c.m_itemType = tr("*");
        m_conditions->insert(row + i, c);
    }
    endInsertRows();
    return true;
}

bool ConditionsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_conditions->removeAt(row + i);
    endRemoveRows();
    return true;
}

}
}
