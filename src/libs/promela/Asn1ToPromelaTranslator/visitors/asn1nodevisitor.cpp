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

#include "asn1nodevisitor.h"

#include "asn1itemtypevisitor.h"
#include "valueassignmentvisitor.h"

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>

using Asn1Acn::Definitions;
using Asn1Acn::File;
using Asn1Acn::Project;
using Asn1Acn::Root;
using Asn1Acn::TypeAssignment;
using Asn1Acn::ValueAssignment;
using conversion::Escaper;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::InlineDef;
using promela::model::PromelaModel;
using promela::model::Sequence;
using promela::model::UtypeRef;
using promela::model::VariableRef;

namespace promela::translator {
Asn1NodeVisitor::Asn1NodeVisitor(PromelaModel &promelaModel, bool enhancedSpinSupport)
    : m_promelaModel(promelaModel)
    , m_enhancedSpinSupport(enhancedSpinSupport)
{
}

void Asn1NodeVisitor::visit(const Definitions &defs)
{
    for (const std::unique_ptr<TypeAssignment> &type : defs.types()) {
        type->accept(*this);
    }

    for (const std::unique_ptr<ValueAssignment> &value : defs.values()) {
        value->accept(*this);
    }
}

void Asn1NodeVisitor::visit(const File &file)
{
    for (const std::unique_ptr<Definitions> &def : file.definitionsList()) {
        def->accept(*this);
    }
}

void Asn1NodeVisitor::visit(const TypeAssignment &type)
{
    Asn1ItemTypeVisitor typeVisitor(m_promelaModel, std::nullopt, type.name(), m_enhancedSpinSupport);
    type.type()->accept(typeVisitor);
}

void Asn1NodeVisitor::visit(const ValueAssignment &value)
{
    const Asn1Acn::Types::Type *type = value.type();

    if (type->typeEnum() == Asn1Acn::Types::Type::USERDEFINED) {
        const Asn1Acn::Types::UserdefinedType *t = dynamic_cast<const Asn1Acn::Types::UserdefinedType *>(type);
        const QString valueName = Escaper::escapePromelaName(value.name());
        const QString typeName = Escaper::escapePromelaName(t->typeName());
        m_promelaModel.addDeclaration(Declaration(DataType(UtypeRef(typeName)), valueName));

        const QString initInlineName = QString("%1_init").arg(valueName);
        const QList<QString> initInlineArgs;
        Sequence initInlineSequence(Sequence::Type::D_STEP);

        VariableRef target(valueName);
        ValueAssignmentVisitor visitor(value.value().get(), initInlineSequence, target, typeName);

        value.type()->accept(visitor);

        m_promelaModel.addInlineDef(
                std::make_unique<InlineDef>(initInlineName, initInlineArgs, std::move(initInlineSequence)));
    }
}

void Asn1NodeVisitor::visit(const Project &project)
{
    Q_UNUSED(project);
}

void Asn1NodeVisitor::visit(const Root &root)
{
    Q_UNUSED(root);
}
}
