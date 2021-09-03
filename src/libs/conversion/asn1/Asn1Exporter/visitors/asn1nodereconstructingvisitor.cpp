/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "asn1nodereconstructingvisitor.h"

#include <QString>

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/root.h>

#include "asn1typecomponentreconstructingvisitor.h"

using namespace Asn1Acn;

Asn1NodeReconstructingVisitor::Asn1NodeReconstructingVisitor(QTextStream &outStream)
    : m_outStream(outStream)
{}

void Asn1NodeReconstructingVisitor::visit(const Data::Definitions &defs)
{
    m_outStream << defs.name() << QStringLiteral(" DEFINITIONS ::= BEGIN") << QStringLiteral("\n");

    reconstructImports(defs);
    reconstructCollection<Data::Definitions::Values>(defs.values());
    reconstructCollection<Data::Definitions::Types>(defs.types());

    m_outStream << QStringLiteral("END\n\n");
}

void Asn1NodeReconstructingVisitor::visit(const Data::File &file)
{
    reconstructCollection<Data::File::DefinitionsList>(file.definitionsList());
}

void Asn1NodeReconstructingVisitor::visit(const Data::TypeAssignment &type)
{
    m_outStream << type.name() << QStringLiteral(" ::= ");

    Asn1TypeComponentReconstructingVisitor visitor(m_outStream);
    type.type()->accept(visitor);

    m_outStream << QStringLiteral("\n");
}

void Asn1NodeReconstructingVisitor::visit(const Data::ValueAssignment &assignment)
{
    if (assignment.value() == nullptr)
        return;

    m_outStream << assignment.name() << QStringLiteral(" ") << assignment.type()->name()
                << QStringLiteral(" ::= ") << assignment.value()->asString()
                << QStringLiteral("\n");
}

void Asn1NodeReconstructingVisitor::visit(const Data::Project &project)
{
    Q_UNUSED(project);
}

void Asn1NodeReconstructingVisitor::visit(const Data::Root &root)
{
    Q_UNUSED(root);
}

void Asn1NodeReconstructingVisitor::reconstructImports(const Data::Definitions &defs) const
{
    const auto &values = defs.importedValues();
    const auto &types = defs.importedTypes();

    if (values.empty() && types.empty())
        return;

    m_outStream << QStringLiteral("IMPORTS\n");

    reconstructImportedCollection<Data::Definitions::ImportedValues>(values);
    reconstructImportedCollection<Data::Definitions::ImportedTypes>(types);

    m_outStream << QStringLiteral(";\n");
}

template<typename T>
void Asn1NodeReconstructingVisitor::reconstructImportedCollection(const T &types) const
{
    for (const auto &type : types)
        m_outStream << type.name() << QStringLiteral(" FROM ") << type.module()
                    << QStringLiteral("\n");
}

template<typename T>
void Asn1NodeReconstructingVisitor::reconstructCollection(const T &collection) const
{
    for (const auto &item : collection) {
        Asn1NodeReconstructingVisitor visitor(m_outStream);
        item->accept(visitor);
    }
}
