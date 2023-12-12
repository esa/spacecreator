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

#include "requirementsmodelbase.h"

namespace requirement {

RequirementsModelBase::RequirementsModelBase(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void RequirementsModelBase::clear()
{
    setRequirements({});
}

/*!
 * Replaces the set of existing requirements with the given one
 */
void RequirementsModelBase::setRequirements(const QList<Requirement> &requirements)
{
    beginResetModel();
    m_requirements = requirements;
    endResetModel();
}

/*!
 * Appends the given \a requiremnets to the existing ones
 */
void RequirementsModelBase::addRequirements(const QList<Requirement> &requirements)
{
    beginInsertRows(QModelIndex(), m_requirements.size(), m_requirements.size() + requirements.size() - 1);
    m_requirements.append(requirements);
    endInsertRows();
}

QVariant RequirementsModelBase::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == REQUIREMENT_ID) {
            return tr("ID");
        }
        if (section == TITLE) {
            return tr("Title");
        }
        if (section == CHECKED) {
            return tr("Selected");
        }
    }

    return {};
}

int RequirementsModelBase::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_requirements.size();
}

int RequirementsModelBase::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant RequirementsModelBase::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_requirements.size()) {
        return QVariant();
    }

    if (role == RequirementsModelBase::RoleNames::IssueLinkRole) {
        return m_requirements[index.row()].m_link;
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case REQUIREMENT_ID:
            return m_requirements[index.row()].m_id;
        case TITLE:
            return m_requirements[index.row()].m_longName;
        }
    }
    if (role == Qt::ToolTipRole) {
        return m_requirements[index.row()].m_description;
    }

    if (role == Qt::CheckStateRole && index.column() == CHECKED) {
        return m_selectedRequirements.contains(getReqIfIdFromModelIndex(index)) ? Qt::Checked : Qt::Unchecked;
    }

    if (role == RequirementsModelBase::RoleNames::ReqIfIdRole) {
        return m_requirements[index.row()].m_id;
    }

    if (role == RequirementsModelBase::RoleNames::IssueIdRole) {
        return m_requirements[index.row()].m_issueID;
    }

    if (role == RequirementsModelBase::RoleNames::TagsRole) {
        return m_requirements[index.row()].m_tags;
    }

    if (role == RequirementsModelBase::RoleNames::DetailDescriptionRole) {
        return m_requirements[index.row()].m_description;
    }

    return QVariant();
}

bool RequirementsModelBase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::CheckStateRole && index.column() == CHECKED) {
        const QString requirementID = getReqIfIdFromModelIndex(index);
        bool checked = value.toBool();
        if (checked) {
            m_selectedRequirements.insert(requirementID);
        } else {
            m_selectedRequirements.remove(requirementID);
        }
        emit dataChanged(index, index, { role });
        return true;
    }

    return QAbstractTableModel::setData(index, value, role);
}

QString RequirementsModelBase::getReqIfIdFromModelIndex(const QModelIndex &index) const
{
    auto idx = this->index(index.row(), REQUIREMENT_ID);
    return this->data(idx, RequirementsModelBase::ReqIfIdRole).toString();
}

Qt::ItemFlags RequirementsModelBase::flags(const QModelIndex &index) const
{
    auto flags = QAbstractTableModel::flags(index);

    if (index.column() == CHECKED) {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

const QSet<QString> &RequirementsModelBase::selectedRequirements() const
{
    return m_selectedRequirements;
}

void RequirementsModelBase::setSelectedRequirements(const QSet<QString> &selected)
{
    beginResetModel();
    m_selectedRequirements = selected;
    endResetModel();
}

QSet<QString> RequirementsModelBase::convert(const QStringList &list)
{
    return QSet<QString>(list.begin(), list.end());
}

Requirement RequirementsModelBase::requirementFromIndex(const QModelIndex &idx)
{
    QModelIndex _idx = index(idx.row(), RequirementsModelBase::REQUIREMENT_ID);
    int issueID = _idx.data(RequirementsModelBase::IssueIdRole).toInt();
    for (const auto &requirement : m_requirements) {
        if (requirement.m_issueID == issueID) {
            return requirement;
        }
    }
    return Requirement();
}

} // namespace requirement
