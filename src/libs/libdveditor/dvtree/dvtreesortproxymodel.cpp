/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvtreesortproxymodel.h"

#include "dvobject.h"
#include "dvtreeviewmodel.h"

#include <QDebug>

namespace dve {

DVTreeSortProxyModel::DVTreeSortProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool DVTreeSortProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QString leftString = sourceModel()->data(left).toString();
    QString rightString = sourceModel()->data(right).toString();

    auto leftObj = sourceModel()->data(left, DVTreeViewModel::DVObjectRole).value<dvm::DVObject *>();
    auto rightObj = sourceModel()->data(right, DVTreeViewModel::DVObjectRole).value<dvm::DVObject *>();
    if (leftObj && rightObj) {
        QString leftPrefix = QString("%1_").arg(static_cast<int>(leftObj->type()), 6, 10, QChar('0'));
        leftString = leftPrefix + leftString;
        QString rightPrefix = QString("%1_").arg(static_cast<int>(rightObj->type()), 6, 10, QChar('0'));
        rightString = rightPrefix + rightString;
    }

    return QString::compare(leftString, rightString, Qt::CaseInsensitive) >= 0;
}

SelectionOverProxyModel::SelectionOverProxyModel(QSortFilterProxyModel *proxyModel, QObject *parent)
    : QItemSelectionModel(proxyModel, parent)
    , m_proxyModel(proxyModel)
{
}

void SelectionOverProxyModel::select(const QItemSelection &selection, SelectionFlags command)
{
    QItemSelectionModel::select(m_proxyModel->mapSelectionFromSource(selection), command);
}

void SelectionOverProxyModel::select(const QModelIndex &index, SelectionFlags command)
{
    QItemSelectionModel::select(m_proxyModel->mapFromSource(index), command);
}

} // namespace dve
