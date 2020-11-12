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
#include "displayrolevisitor.h"

#include <asn1/definitions.h>
#include <asn1/file.h>
#include <asn1/project.h>
#include <asn1/typeassignment.h>
#include <asn1/valueassignment.h>

using namespace Asn1Acn::Internal::TreeViews;

DisplayRoleVisitor::~DisplayRoleVisitor() {}

QString DisplayRoleVisitor::valueFor(const Asn1Acn::Definitions &defs) const
{
    return defs.name();
}

QString DisplayRoleVisitor::valueFor(const Asn1Acn::File &file) const
{
    return file.location().path();
}

QString DisplayRoleVisitor::valueFor(const Asn1Acn::TypeAssignment &type) const
{
    return type.name() + type.type()->label();
}

QString DisplayRoleVisitor::valueFor(const Asn1Acn::ValueAssignment &value) const
{
    return value.name() + value.type()->label();
}

QString DisplayRoleVisitor::valueFor(const Asn1Acn::Project &project) const
{
    return project.name();
}

QString DisplayRoleVisitor::valueFor(const Asn1Acn::Root &root) const
{
    Q_UNUSED(root);
    return {};
}
