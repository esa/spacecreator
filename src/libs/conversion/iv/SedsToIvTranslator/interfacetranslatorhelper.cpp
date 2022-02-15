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

#include "interfacetranslatorhelper.h"

#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/sequence.h>
#include <conversion/asn1/SedsToAsn1Translator/specialized/datatypetranslatorvisitor.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/dimensionsize.h>
#include <shared/parameter.h>

using conversion::UnhandledValueException;
using conversion::translator::TranslationException;

namespace conversion::iv::translator {

const QString InterfaceTranslatorHelper::m_interfaceParameterEncoding = "ACN";
const QString InterfaceTranslatorHelper::m_ivParameterInterfaceNameTemplate = "%1_%2_%3_%4";
const QString InterfaceTranslatorHelper::m_ivCommandInterfaceNameTemplate = "%1_%2_%3";
const QString InterfaceTranslatorHelper::m_asn1ArrayNameTemplate = "%1Array-%2";
const QString InterfaceTranslatorHelper::m_bundledTypeNameTemplate = "%1-Type%2";
const QString InterfaceTranslatorHelper::m_alternateTypeNameTemplate = "%1-%2";
const QString InterfaceTranslatorHelper::m_getterInterfacePrefix = "Get";
const QString InterfaceTranslatorHelper::m_setterInterfacePrefix = "Set";

shared::InterfaceParameter InterfaceTranslatorHelper::createInterfaceParameter(const QString &parameterName,
        const QString &sedsParameterTypeName, shared::InterfaceParameter::Direction direction)
{
    return shared::InterfaceParameter(parameterName, shared::BasicParameter::Type::Other,
            Escaper::escapeIvName(sedsParameterTypeName), m_interfaceParameterEncoding, direction);
}

ivm::IVInterface *InterfaceTranslatorHelper::createIvInterface(const QString &name,
        ivm::IVInterface::InterfaceType type, ivm::IVInterface::OperationKind kind, ivm::IVFunction *m_ivFunction)
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.name = name;
    creationInfo.type = type;
    creationInfo.kind = kind;
    creationInfo.function = m_ivFunction;

    return ivm::IVInterface::createIface(creationInfo);
}

QString InterfaceTranslatorHelper::createArrayType(const QString &baseTypeName,
        const std::vector<seds::model::DimensionSize> &dimensions, Asn1Acn::Definitions *asn1Definitions,
        const seds::model::Package *sedsPackage)
{
    const auto name = buildArrayTypeName(baseTypeName, dimensions);

    const auto foundAsn1Array = asn1Definitions->type(name);
    if (foundAsn1Array != nullptr) {
        return name;
    }

    seds::model::ArrayDataType sedsArray;
    sedsArray.setName(name);
    sedsArray.setType(baseTypeName);

    for (auto dimension : dimensions) {
        sedsArray.addDimension(std::move(dimension));
    }

    std::unique_ptr<Asn1Acn::Types::Type> asn1Array;
    asn1::translator::DataTypeTranslatorVisitor dataTypeVisitor(asn1Array, asn1Definitions, sedsPackage);
    dataTypeVisitor(sedsArray);

    auto asn1ArrayAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(asn1Array));
    asn1Definitions->addType(std::move(asn1ArrayAssignment));

    return name;
}

QString InterfaceTranslatorHelper::buildParameterInterfaceName(const QString &sedsInterfaceName,
        const QString &sedsParameterName,
        const InterfaceTranslatorHelper::InterfaceParameterType interfaceParameterType,
        ivm::IVInterface::InterfaceType type)
{
    switch (interfaceParameterType) {
    case InterfaceParameterType::Getter:
        return Escaper::escapeIvName(m_ivParameterInterfaceNameTemplate.arg(m_getterInterfacePrefix)
                                             .arg(sedsInterfaceName)
                                             .arg(sedsParameterName)
                                             .arg(interfaceTypeToString(type)));
    case InterfaceParameterType::Setter:
        return Escaper::escapeIvName(m_ivParameterInterfaceNameTemplate.arg(m_setterInterfacePrefix)
                                             .arg(sedsInterfaceName)
                                             .arg(sedsParameterName)
                                             .arg(interfaceTypeToString(type)));
    default:
        throw UnhandledValueException("InterfaceMode");
        break;
    }
}

QString InterfaceTranslatorHelper::buildCommandInterfaceName(
        const QString &sedsInterfaceName, const ivm::IVInterface::InterfaceType type, const QString &commandName)
{
    return Escaper::escapeIvName(
            m_ivCommandInterfaceNameTemplate.arg(sedsInterfaceName).arg(commandName).arg(interfaceTypeToString(type)));
}

QString InterfaceTranslatorHelper::buildArrayTypeName(
        const QString &baseTypeName, const std::vector<seds::model::DimensionSize> &dimensions)
{
    const auto dimensionSizeFunc = [&](const seds::model::DimensionSize &dimension) {
        if (dimension.size()) {
            return QString::number(dimension.size()->value());
        } else if (dimension.indexTypeRef()) {
            return dimension.indexTypeRef()->nameStr();
        } else {
            return QString();
        }
    };

    const auto dimensionSuffix = std::accumulate(std::next(dimensions.begin()), dimensions.end(),
            dimensionSizeFunc(dimensions.front()), [&](const auto &acc, const seds::model::DimensionSize &dimension) {
                return acc + "x" + dimensionSizeFunc(dimension);
            });

    return Escaper::escapeAsn1TypeName(m_asn1ArrayNameTemplate.arg(baseTypeName).arg(dimensionSuffix));
}

QString InterfaceTranslatorHelper::buildBundledTypeName(
        const QString &sedsCommandName, const std::size_t cachedTypesCount)
{
    const auto sedsCommandNameEscaped = Escaper::escapeAsn1TypeName(sedsCommandName);

    if (cachedTypesCount == 0) {
        return m_bundledTypeNameTemplate.arg(sedsCommandNameEscaped).arg("");
    } else {
        return m_bundledTypeNameTemplate.arg(sedsCommandNameEscaped).arg(cachedTypesCount);
    }
}

QString InterfaceTranslatorHelper::buildAlternateTypeName(
        const QString &sedsInterfaceName, const QString &genericTypeName)
{
    return m_alternateTypeNameTemplate.arg(sedsInterfaceName).arg(genericTypeName);
}

ivm::IVInterface::InterfaceType InterfaceTranslatorHelper::switchInterfaceType(
        ivm::IVInterface::InterfaceType interfaceType)
{
    switch (interfaceType) {
    case ivm::IVInterface::InterfaceType::Required:
        return ivm::IVInterface::InterfaceType::Provided;
    case ivm::IVInterface::InterfaceType::Provided:
        return ivm::IVInterface::InterfaceType::Required;
    default:
        throw UnhandledValueException("InterfaceType");
        break;
    }
}

const QString &InterfaceTranslatorHelper::interfaceTypeToString(ivm::IVInterface::InterfaceType type)
{
    switch (type) {
    case ivm::IVInterface::InterfaceType::Required: {
        static QString name = "Ri";
        return name;
    }
    case ivm::IVInterface::InterfaceType::Provided: {
        static QString name = "Pi";
        return name;
    }
    case ivm::IVInterface::InterfaceType::Grouped: {
        static QString name = "Grp";
        return name;
    }
    default:
        throw UnhandledValueException("ivm::InterfaceType");
        break;
    }
}

} // namespace conversion::iv::translator
