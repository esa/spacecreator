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

#include "leafdocumentsmodel.h"

namespace aadlinterface {

LeafDocumentsModel::LeafDocumentsModel(QObject *parent)
    : QObjectListModelT<msc::MscDocument *>(parent)
{
}

void LeafDocumentsModel::fillModel(msc::MscDocument *rootDoc)
{
    clear();
    for (msc::MscDocument *doc : rootDoc->documents()) {
        if (doc->hierarchyType() == msc::MscDocument::HierarchyLeaf) {
            append(doc);
        } else {
            fillModel(doc);
        }
    }
}

QVariant LeafDocumentsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(at(index.row())->name());
    }

    return QObjectListModelT::data(index, role);
}

}
