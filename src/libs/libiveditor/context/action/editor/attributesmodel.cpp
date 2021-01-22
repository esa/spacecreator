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

#include "attributesmodel.h"

#include "context/action/attrhandler.h"

namespace ive {

const int COLUMN_ID_Name = 0;
const int COLUMN_ID_Value = 1;

/*!
 * \class ive::AttributesModel
 * \brief The model to store script-able actions attributes in editor dialog.
 */
AttributesModel::AttributesModel(QObject *parent)
    : QStringListModel(parent)
{
}

void AttributesModel::setAttributesList(QVector<AttrHandler> *attrs)
{
    if (attrs != m_attrs) {
        beginResetModel();
        m_attrs = attrs;
        endResetModel();
    }
}

QVariant AttributesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case COLUMN_ID_Name:
            return tr("Name");
        case COLUMN_ID_Value:
            return tr("Value");
        default:
            break;
        }
    }
    return QStringListModel::headerData(section, orientation, role);
}

int AttributesModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return m_attrs ? m_attrs->size() : 0;
}

int AttributesModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant AttributesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (m_attrs)
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole: {
            switch (index.column()) {
            case COLUMN_ID_Name:
                return m_attrs->at(index.row()).m_title;
            case COLUMN_ID_Value:
                return m_attrs->at(index.row()).m_value;
            default:
                break;
            }
        }
        default:
            break;
        }

    return QStringListModel::data(index, role);
}

bool AttributesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    bool changed(false);

    if (data(index, role) != value && m_attrs) {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            AttrHandler &attr = m_attrs->operator[](index.row());
            switch (index.column()) {
            case COLUMN_ID_Name: {
                attr.m_title = value.toString();
                changed = true;
                break;
            }
            case COLUMN_ID_Value: {
                attr.m_value = value;
                changed = true;
                break;
            }
            default:
                break;
            }
        }
    }

    if (changed) {
        Q_EMIT dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return QStringListModel::setData(index, value, role);
}

bool AttributesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (!m_attrs)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        AttrHandler attr;
        attr.m_title = tr("Attribute");
        attr.m_value = tr("*");
        m_attrs->insert(row + i, attr);
    }
    endInsertRows();
    return true;
}

bool AttributesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!m_attrs)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_attrs->removeAt(row + i);
    endRemoveRows();
    return true;
}

}
