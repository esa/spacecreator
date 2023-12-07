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

#include "requirementtextproxymodel.h"

#include "requirementsmodelbase.h"

namespace requirement {

RequirementTextProxyModel::RequirementTextProxyModel(QObject *parent)
    : QSortFilterProxyModel { parent }
{
}

bool RequirementTextProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QRegularExpression expression = filterRegularExpression();

    const QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);

    const QString reqId = idx.data(RequirementsModelBase::ReqIfIdRole).toString();
    QRegularExpressionMatch match = expression.match(reqId);
    if (match.hasMatch()) {
        return true;
    }
    const QString detail = idx.data(RequirementsModelBase::DetailDescriptionRole).toString();
    match = expression.match(detail);
    if (match.hasMatch()) {
        return true;
    }
    const QModelIndex titleIdx = sourceModel()->index(sourceRow, RequirementsModelBase::TITLE, sourceParent);
    const QString title = titleIdx.data(Qt::DisplayRole).toString();
    match = expression.match(title);
    if (match.hasMatch()) {
        return true;
    }

    return false;
}

} // namespace requirement
