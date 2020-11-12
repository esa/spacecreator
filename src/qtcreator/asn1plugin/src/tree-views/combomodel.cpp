/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "combomodel.h"

#include "combo-visitors/childrencountingvisitor.h"
#include "combo-visitors/childreturningvisitor.h"
#include "combo-visitors/indexfindingvisitor.h"

using namespace Asn1Acn::Internal::TreeViews::ComboVisitors;
using namespace Asn1Acn::Internal::TreeViews;

ComboModel::ComboModel(const Utils::FileName &filePath, QObject *parent)
    : MutableRootModel(filePath, parent)
{}

ComboModel::~ComboModel() {}

Asn1Acn::Node *ComboModel::parentOf(const Asn1Acn::Node *node) const
{
    return node ? node->parent() : nullptr;
}

int ComboModel::childrenCount(const Asn1Acn::Node *node) const
{
    return node ? node->valueFor<ChildrenCountingVisitor>() : 0;
}

int ComboModel::indexInParent(const Asn1Acn::Node *parent, const Asn1Acn::Node *node) const
{
    return parent ? parent->valueFor<IndexFindingVisitor>(node) : 0;
}

Asn1Acn::Node *ComboModel::nthChild(const Asn1Acn::Node *node, int n) const
{
    return node ? node->valueFor<ChildReturningVisitor>(n) : nullptr;
}
