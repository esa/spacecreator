/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "datatypetranslationhelper.h"

#include "specialized/datatypetranslatorvisitor.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>

using conversion::translator::MissingAsn1TypeDefinitionException;

namespace conversion::asn1::translator {

QString DataTypeTranslationHelper::buildArrayTypeName(
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

    return Escaper::escapeAsn1TypeName(m_arrayTypeNameTemplate.arg(baseTypeName).arg(dimensionSuffix));
}

QString DataTypeTranslationHelper::buildBundledTypeName(
        const QString &interfaceDeclarationName, const QString &commandName, QString postfix)
{
    return m_bundledTypeNameTemplate.arg(interfaceDeclarationName).arg(commandName).arg(postfix);
}

QString DataTypeTranslationHelper::buildGenericBundledTypeName(
        const QString &componentName, const QString &interfaceName, const QString &commandName)
{
    return m_genericBundledTypeNameTemplate.arg(componentName).arg(interfaceName).arg(commandName);
}

QString DataTypeTranslationHelper::buildConcreteTypeName(
        const QString &interfaceDeclarationName, const QString &genericName)
{
    return m_concreteTypeNameDeclarationTemplate.arg(interfaceDeclarationName).arg(genericName);
}

QString DataTypeTranslationHelper::buildConcreteTypeName(
        const QString &componentName, const QString &interfaceName, const QString &genericName)
{
    return m_concreteTypeNameImplementationTemplate.arg(componentName).arg(interfaceName).arg(genericName);
}

seds::model::DataTypeRef DataTypeTranslationHelper::createArrayType(Context &context,
        const seds::model::DataTypeRef &baseTypeRef, const std::vector<seds::model::DimensionSize> &dimensions)
{
    const auto name = buildArrayTypeName(baseTypeRef.nameStr(), dimensions);

    if (!context.hasAsn1Type(name)) {
        seds::model::ArrayDataType sedsArray;
        sedsArray.setName(name);
        sedsArray.setType(baseTypeRef);

        for (auto dimension : dimensions) {
            sedsArray.addDimension(std::move(dimension));
        }

        DataTypeTranslatorVisitor dataTypeTranslator(context);
        dataTypeTranslator(sedsArray);
    }

    return name;
}

Asn1Acn::Types::Type *DataTypeTranslationHelper::handleArrayType(Context &context,
        const seds::model::DataTypeRef &argumentTypeRef, const std::vector<seds::model::DimensionSize> &dimensions)
{
    if (dimensions.empty()) {
        return context.findAsn1Type(argumentTypeRef);
    } else {
        const auto name = buildArrayTypeName(argumentTypeRef.nameStr(), dimensions);

        if (!context.hasAsn1Type(name)) {
            seds::model::ArrayDataType sedsArray;
            sedsArray.setName(name);
            sedsArray.setType(argumentTypeRef);

            for (auto dimension : dimensions) {
                sedsArray.addDimension(std::move(dimension));
            }

            DataTypeTranslatorVisitor dataTypeTranslator(context);
            dataTypeTranslator(sedsArray);
        }

        return context.findAsn1Type(name);
    }
}

bool DataTypeTranslationHelper::isTypeGeneric(
        const seds::model::DataTypeRef &typeRef, const std::vector<seds::model::GenericType> &genericTypes)
{
    if (typeRef.packageStr()) {
        return false;
    }

    const auto &typeName = typeRef.nameStr();

    const auto found = std::find_if(genericTypes.begin(), genericTypes.end(),
            [&](const seds::model::GenericType &genericType) { return genericType.nameStr() == typeName; });

    return found != genericTypes.end();
}

} // namespace conversion::asn1::translator
