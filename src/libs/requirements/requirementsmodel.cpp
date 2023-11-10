/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "requirementsmodel.h"

namespace requirement {

RequirementsModel::RequirementsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void RequirementsModel::clear()
{
    setRequirements({});
}

void RequirementsModel::setRequirements(const QList<Requirement> &requirements)
{
    beginResetModel();
    m_requirements = requirements;
    endResetModel();
}

void RequirementsModel::addRequirements(const QList<Requirement> &requirements)
{
    beginInsertRows(QModelIndex(), m_requirements.size(), m_requirements.size() + requirements.size() - 1);

    m_requirements.append(requirements);

    endInsertRows();
}

void RequirementsModel::setSelectedRequirementsIDs(const QStringList &requirementIDs)
{
    m_selected_requirements = QSet(requirementIDs.begin(), requirementIDs.end());
}

QVariant RequirementsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == REQUIREMENT_ID) {
            return tr("ID");
        }
        if (section == DESCRIPTION) {
            return tr("Description");
        }
        if (section == CHECKED) {
            return tr("Selected");
        }
    }

    return {};
}

int RequirementsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_requirements.size();
}

int RequirementsModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant RequirementsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_requirements.size()) {
        return QVariant();
    }

    if (role == RequirementsModel::RoleNames::IssueLinkRole) {
        return m_requirements[index.row()].m_link;
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case REQUIREMENT_ID:
            return m_requirements[index.row()].m_id;
        case DESCRIPTION:
            return m_requirements[index.row()].m_longName;
        }
    }
    if (role == Qt::ToolTipRole) {
        return m_requirements[index.row()].m_description;
    }

    if (role == Qt::CheckStateRole && index.column() == CHECKED) {
        auto requirementID = getIdFromModelIndex(index);
        return (m_selected_requirements.contains(requirementID)) ? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}

bool RequirementsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::CheckStateRole && index.column() == CHECKED) {
        auto requirementID = getIdFromModelIndex(index);
        auto checked = value.toBool();
        if (checked) {
            m_selected_requirements << requirementID;
        } else {
            m_selected_requirements.remove(requirementID);
        }

        emit dataChanged(index, index, { role });
        return true;
    }

    return QAbstractTableModel::setData(index, value, role);
}

QString RequirementsModel::getIdFromModelIndex(const QModelIndex &index) const
{
    auto idx = this->index(index.row(), requirement::RequirementsModel::REQUIREMENT_ID);
    return this->data(idx, Qt::DisplayRole).toString();
}

Qt::ItemFlags RequirementsModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractTableModel::flags(index);

    if (index.column() == CHECKED) {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

} // namespace requirement
