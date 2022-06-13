/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "asn1nodevaluegeneratorvisitor.h"

#include "asn1typevaluegeneratorvisitor.h"

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/file.h>

using Asn1Acn::Definitions;
using Asn1Acn::File;
using Asn1Acn::Project;
using Asn1Acn::Root;
using Asn1Acn::TypeAssignment;
using Asn1Acn::ValueAssignment;
using promela::model::PromelaModel;

namespace promela::translator {
Asn1NodeValueGeneratorVisitor::Asn1NodeValueGeneratorVisitor(PromelaModel &promelaModel, const QStringList &typeNames)
    : m_promelaModel(promelaModel)
    , m_typeNames(typeNames)
{
}

void Asn1NodeValueGeneratorVisitor::visit(const Definitions &defs)
{
    for (const std::unique_ptr<TypeAssignment> &type : defs.types()) {
        type->accept(*this);
    }
}

void Asn1NodeValueGeneratorVisitor::visit(const File &file)
{
    for (const std::unique_ptr<Definitions> &def : file.definitionsList()) {
        def->accept(*this);
    }
}

void Asn1NodeValueGeneratorVisitor::visit(const TypeAssignment &type)
{
    if (m_typeNames.contains(type.name())) {
        Asn1TypeValueGeneratorVisitor typeVisitor(m_promelaModel, type.name());
        type.type()->accept(typeVisitor);
    }
}

void Asn1NodeValueGeneratorVisitor::visit(const ValueAssignment &value)
{
    Q_UNUSED(value);
}

void Asn1NodeValueGeneratorVisitor::visit(const Project &project)
{
    Q_UNUSED(project);
}

void Asn1NodeValueGeneratorVisitor::visit(const Root &root)
{
    Q_UNUSED(root);
}
}
