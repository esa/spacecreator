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
#include "typestreemodel.h"

#include "typestree-visitors/childrencountingvisitor.h"
#include "typestree-visitors/childreturningvisitor.h"
#include "typestree-visitors/indexfindingvisitor.h"
#include "typestree-visitors/parentreturningvisitor.h"

#include "modelvalidityguard.h"

using namespace Asn1Acn::Internal::TreeViews::TypesTreeVisitors;
using namespace Asn1Acn::Internal::TreeViews;

TypesTreeModel::TypesTreeModel(QObject *parent)
    : Model(parent)
{
    connect(ModelValidityGuard::instance(),
            &ModelValidityGuard::modelChanged,
            this,
            &TypesTreeModel::endResetModel);
}

TypesTreeModel::~TypesTreeModel() {}

void TypesTreeModel::onEndResetModel()
{
    endResetModel();
}

Asn1Acn::Node *TypesTreeModel::parentOf(const Asn1Acn::Node *node) const
{
    return node ? node->valueFor<ParentReturningVisitor>() : nullptr;
}

int TypesTreeModel::childrenCount(const Asn1Acn::Node *node) const
{
    return node ? node->valueFor<ChildrenCountingVisitor>() : 0;
}

int TypesTreeModel::indexInParent(const Asn1Acn::Node *parent, const Asn1Acn::Node *node) const
{
    return parent ? parent->valueFor<IndexFindingVisitor>(node) : 0;
}

Asn1Acn::Node *TypesTreeModel::nthChild(const Asn1Acn::Node *node, int n) const
{
    return node ? node->valueFor<ChildReturningVisitor>(n) : nullptr;
}
