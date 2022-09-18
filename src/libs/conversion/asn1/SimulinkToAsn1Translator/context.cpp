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

#include "context.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/simulink/matlabstandarddatatypes.h>
#include <conversion/common/translation/exceptions.h>

using Asn1Acn::Definitions;
using Asn1Acn::ImportedType;
using Asn1Acn::SourceLocation;
using Asn1Acn::TypeAssignment;
using Asn1Acn::Types::Type;
using conversion::Escaper;
using conversion::simulink::MatLabStandardDataTypes;
using conversion::translator::MissingAsn1TypeDefinitionException;
using simulink::model::SimulinkModel;

namespace conversion::asn1::translator {

Context::Context(const SimulinkModel &simulinkModel, std::unique_ptr<Definitions> &matlabDefinitions,
        std::unique_ptr<Definitions> &simulinkModelDefinitions, const Options &options)
    : m_simulinkModel(simulinkModel)
    , m_matlabDefinitions(matlabDefinitions)
    , m_simulinkModelDefinitions(simulinkModelDefinitions)
    , m_options(options)
{
}

void Context::addSimulinkModelAsn1Type(std::unique_ptr<Type> asn1Type, QString description)
{
    const auto &asn1TypeName = asn1Type->identifier();
    auto asn1TypeAssignment =
            std::make_unique<TypeAssignment>(asn1TypeName, asn1TypeName, SourceLocation(), std::move(asn1Type));

    if (!description.isEmpty())
        asn1TypeAssignment->setComment(description);

    m_simulinkModelDefinitions->addType(std::move(asn1TypeAssignment));
}

void Context::addMatLabStandardAsn1Type(std::unique_ptr<Type> asn1Type)
{
    const auto &asn1TypeName = asn1Type->identifier();
    auto asn1TypeAssignment =
            std::make_unique<TypeAssignment>(asn1TypeName, asn1TypeName, SourceLocation(), std::move(asn1Type));

    ImportedType importedType(matlabStandardDataTypesDefinitionsName(), asn1TypeName);
    m_simulinkModelDefinitions->addImportedType(importedType);

    m_matlabDefinitions->addType(std::move(asn1TypeAssignment));
}

Type *Context::findAsn1Type(const QString &dataType)
{
    if (auto *typeAssignment = m_simulinkModelDefinitions->type(Escaper::escapeAsn1TypeName(dataType))) {
        return typeAssignment->type();
    }

    const auto matlabDataTypeName = MatLabStandardDataTypes::buildMatLabDataTypeName(dataType);

    if (auto *matlabTypeAssignment = m_matlabDefinitions->type(matlabDataTypeName)) {
        return matlabTypeAssignment->type();
    }

    throw MissingAsn1TypeDefinitionException(dataType);
}

const QString &Context::simulinkModelDefinitionsName() const
{
    return m_simulinkModelDefinitions->name();
}

const QString &Context::matlabStandardDataTypesDefinitionsName() const
{
    return m_matlabDefinitions->name();
}

Asn1Acn::Definitions *Context::getSimulinkModelAsn1Definitions() const
{
    return m_simulinkModelDefinitions.get();
}

} // namespace conversion::asn1::translator
