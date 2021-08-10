/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "propertiesmodelbase.h"

namespace shared {

PropertiesModelBase::PropertiesModelBase(QObject *parent)
    : QStandardItemModel(parent)
{
}

VEObject *PropertiesModelBase::entity() const
{
    return m_dataObject;
}

bool PropertiesModelBase::moveRows(const QModelIndex &sourceParent, int sourceRow,
        int count, const QModelIndex &destinationParent, int destinationChild)
{
    for (int idx = sourceRow; idx < sourceRow + count; ++idx) {
        auto items = takeRow(idx);
        insertRow(destinationChild + idx - sourceRow, items);
    }
    return true;
}

} // namespace shared
