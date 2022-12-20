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

#include <asn1library/asn1/constraints/withconstraints.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>

using Asn1Acn::Constraints::WithConstraints;
using conversion::translator::MissingAsn1TypeDefinitionException;

namespace conversion::asn1::translator::seds {

QString DataTypeTranslationHelper::buildArrayTypeName(
        const QString &baseTypeName, const std::vector<::seds::model::DimensionSize> &dimensions)
{
    const auto dimensionSizeFunc = [&](const ::seds::model::DimensionSize &dimension) {
        if (dimension.size()) {
            return QString::number(dimension.size()->value());
        } else if (dimension.indexTypeRef()) {
            return dimension.indexTypeRef()->nameStr();
        } else {
            return QString();
        }
    };

    const auto dimensionSuffix = std::accumulate(std::next(dimensions.begin()), dimensions.end(),
            dimensionSizeFunc(dimensions.front()), [&](const auto &acc, const ::seds::model::DimensionSize &dimension) {
                return acc + "x" + dimensionSizeFunc(dimension);
            });

    return Escaper::escapeAsn1TypeName(m_arrayTypeNameTemplate.arg(baseTypeName).arg(dimensionSuffix));
}

QString DataTypeTranslationHelper::buildBundledTypeName(
        const QString &parentName, const QString &commandName, const ::seds::model::CommandArgumentMode mode)
{
    QString postfix;
    if (mode == ::seds::model::CommandArgumentMode::Notify) {
        postfix = "Notify";
    }

    return m_bundledTypeNameTemplate.arg(parentName).arg(commandName).arg(postfix);
}

QString DataTypeTranslationHelper::buildConcreteTypeName(
        const QString &interfaceDeclarationName, const QString &genericName)
{
    return m_concreteTypeNameDeclarationTemplate.arg(interfaceDeclarationName).arg(genericName);
}

::seds::model::DataTypeRef DataTypeTranslationHelper::createArrayType(Context &context,
        const ::seds::model::DataTypeRef &typeRef, const std::vector<::seds::model::DimensionSize> &dimensions)
{
    if (dimensions.empty()) {
        return typeRef;
    }

    const auto name = buildArrayTypeName(typeRef.nameStr(), dimensions);

    if (!context.hasAsn1Type(name)) {
        ::seds::model::ArrayDataType sedsArray;
        sedsArray.setName(name);
        sedsArray.setType(typeRef);

        for (auto dimension : dimensions) {
            sedsArray.addDimension(std::move(dimension));
        }

        DataTypeTranslatorVisitor dataTypeTranslator(context);
        dataTypeTranslator(sedsArray);
    }

    return name;
}

Asn1Acn::Types::Type *DataTypeTranslationHelper::handleArrayType(Context &context,
        const ::seds::model::DataTypeRef &typeRef, const std::vector<::seds::model::DimensionSize> &dimensions)
{
    if (dimensions.empty()) {
        return context.findAsn1Type(typeRef);
    } else {
        const auto name = createArrayType(context, typeRef, dimensions);
        return context.findAsn1Type(name);
    }
}

void DataTypeTranslationHelper::removeConstraints(Asn1Acn::Types::Type *asn1Type)
{
    switch (asn1Type->typeEnum()) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::IntegerValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::REAL: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::RealValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::BooleanValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::SEQUENCEOF: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::IntegerValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::ENUMERATED: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::EnumValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::IA5STRING: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::StringValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::NUMERICSTRING: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::StringValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::BITSTRING: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::BitStringValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::OCTETSTRING: {
        auto asn1WithConstraints = dynamic_cast<WithConstraints<Asn1Acn::OctetStringValue> *>(asn1Type);
        asn1WithConstraints->removeConstraints();
    } break;
    case Asn1Acn::Types::Type::ASN1Type::USERDEFINED: {
        auto asn1UserdefinedType = dynamic_cast<Asn1Acn::Types::UserdefinedType *>(asn1Type);
        removeConstraints(asn1UserdefinedType->type());
    } break;
    default:
        break;
    }
}

bool DataTypeTranslationHelper::isTypeGeneric(
        const ::seds::model::DataTypeRef &typeRef, const std::vector<::seds::model::GenericType> &genericTypes)
{
    if (typeRef.packageStr()) {
        return false;
    }

    const auto &typeName = typeRef.nameStr();

    const auto found = std::find_if(genericTypes.begin(), genericTypes.end(),
            [&](const ::seds::model::GenericType &genericType) { return genericType.nameStr() == typeName; });

    return found != genericTypes.end();
}

} // namespace conversion::asn1::translator::seds
