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

QVariant RequirementsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return tr("ID");
        }
        if (section == 1) {
            return tr("Description");
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
  return 2;
}

QVariant RequirementsModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.column() >= m_requirements.size()) {
      return QVariant();
  }

  if (role == RequirementsModel::RoleNames::IssueLinkRole) {
      switch (index.column()) {
      case 0:
          return m_requirements[index.row()].m_id;
      case 1:
          return m_requirements[index.row()].m_link;
      }
  }
  if (role == Qt::DisplayRole) {
      switch (index.column()) {
      case 0:
          return m_requirements[index.row()].m_id;
      case 1:
          return m_requirements[index.row()].m_longName;
      }
  }
  if (role == Qt::ToolTipRole) {
      return m_requirements[index.row()].m_description;
  }
  return QVariant();
}

} // namespace requirement
