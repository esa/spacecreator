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

#include "acnnodereconstructingvisitor.h"

#include "acntypecomponentreconstructingvisitor.h"

#include <QString>
#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/root.h>

using namespace Asn1Acn;

AcnNodeReconstructingVisitor::AcnNodeReconstructingVisitor(QTextStream &outStream)
    : m_outStream(outStream)
{
}

void AcnNodeReconstructingVisitor::visit(const Data::Definitions &defs)
{
    m_outStream << defs.name() << QStringLiteral(" DEFINITIONS ::= BEGIN") << QStringLiteral("\n");
    reconstructCollection(defs.types());
    m_outStream << QStringLiteral("END\n\n");
}

void AcnNodeReconstructingVisitor::visit(const Data::File &file)
{
    reconstructCollection<Data::File::DefinitionsList>(file.definitionsList());
}

void AcnNodeReconstructingVisitor::visit(const Data::TypeAssignment &type)
{
    m_outStream << type.name() << QStringLiteral(" ");

    AcnTypeComponentReconstructingVisitor visitor(m_outStream);
    type.type()->accept(visitor);

    m_outStream << QStringLiteral("\n");
}

void AcnNodeReconstructingVisitor::visit(const Data::ValueAssignment &assignment)
{
    Q_UNUSED(assignment);
}

void AcnNodeReconstructingVisitor::visit(const Data::Project &project)
{
    Q_UNUSED(project);
}

void AcnNodeReconstructingVisitor::visit(const Data::Root &root)
{
    Q_UNUSED(root);
}

template<typename T>
void AcnNodeReconstructingVisitor::reconstructCollection(const T &collection) const
{
    for (const auto &item : collection) {
        AcnNodeReconstructingVisitor visitor(m_outStream);
        item->accept(visitor);
    }
}
