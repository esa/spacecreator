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
#include <conversion/common/translation/exceptions.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::Definitions;
using Asn1Acn::File;
using Asn1Acn::Project;
using Asn1Acn::Root;
using Asn1Acn::TypeAssignment;
using Asn1Acn::ValueAssignment;
using Asn1Acn::Types::Type;
using conversion::translator::TranslationException;
using promela::model::PromelaModel;

namespace promela::translator {

Asn1NodeValueGeneratorVisitor::Asn1NodeValueGeneratorVisitor(PromelaModel &promelaModel, const Asn1Model *asn1Model)
    : m_promelaModel(promelaModel)
    , m_asn1Model(asn1Model)
    , m_generateSubtypes(true)
{
}

Asn1NodeValueGeneratorVisitor::Asn1NodeValueGeneratorVisitor(PromelaModel &promelaModel, QStringList typeNames)
    : m_promelaModel(promelaModel)
    , m_asn1Model(nullptr)
    , m_typeNames(std::move(typeNames))
    , m_generateSubtypes(false)
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
    if (m_generateSubtypes) {
        const auto overridenType = findOverridenType(type.name());

        Asn1TypeValueGeneratorVisitor typeVisitor(m_promelaModel, type.name(), overridenType);
        type.type()->accept(typeVisitor);
    } else if (m_typeNames.contains(type.name())) {
        Asn1TypeValueGeneratorVisitor typeVisitor(m_promelaModel, type.name(), nullptr);
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

const Type *Asn1NodeValueGeneratorVisitor::findOverridenType(const QString &subtypeName) const
{
    Q_UNUSED(subtypeName);

    for (const auto &asn1File : m_asn1Model->data()) {
        const auto foundType = asn1File->typeFromName("MyOctoString");
        if (foundType != nullptr) {
            return foundType;
        }
    }

    auto errorMessage = QString("Unable to find argument %1 in interface %2 in function %3 used in subtyping")
                                .arg("")
                                .arg("")
                                .arg("");
    throw TranslationException(std::move(errorMessage));
    return nullptr;
}

}
