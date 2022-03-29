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

#include "datareconstructor.h"

namespace testgenerator {

QVector<QVariant> DataReconstructor::getVariantVectorFromRawData(QByteArray rawData,
        const unsigned int TestVectorsNumber, ivm::IVInterface *const iface, Asn1Acn::Asn1Model *const asn1Model)
{
    QVector<QVariant> output;
    output.reserve(static_cast<int>(TestVectorsNumber));

    const auto map = mapAsnTypesToPlatformTypes(asn1Model);

    for (unsigned int i = 0; i < TestVectorsNumber; i++) {
        for (const auto &param : iface->params()) {
            const auto &type = map.value(param.paramTypeName());
            if (type == nullptr) {
                throw std::runtime_error("requested type cannot be found");
            }

            const int sizeOfVar = getSizeOfType(type->typeEnum());
            const QByteArray rawVar = popFrontQByteArray(sizeOfVar, rawData);

            const auto typeEnum = type->typeEnum();
            if (typeEnum == Asn1Acn::Types::Type::ASN1Type::INTEGER) {
                pushBackCopyToVariantVector<int>(output, rawVar);
            } else if (typeEnum == Asn1Acn::Types::Type::ASN1Type::REAL) {
                pushBackCopyToVariantVector<double>(output, rawVar);
            } else if (typeEnum == Asn1Acn::Types::Type::ASN1Type::BOOLEAN) {
                pushBackCopyToVariantVector<bool>(output, rawVar);
            }
        }
    }

    return output;
}

QMap<QString, Asn1Acn::Types::Type *> DataReconstructor::mapAsnTypesToPlatformTypes(Asn1Acn::Asn1Model *const model)
{
    QMap<QString, Asn1Acn::Types::Type *> output;

    for (const auto &file : model->data()) {
        if (file == nullptr) {
            continue;
        }

        for (const auto &definition : file->definitionsList()) {
            if (definition == nullptr) {
                continue;
            }
            const auto &definitionTypeAssignmentNames = definition->typeAssignmentNames();
            if (definitionTypeAssignmentNames.isEmpty()) {
                continue;
            }
            if (definitionTypeAssignmentNames.size() == 1) {
                output.insert(definition->types().front()->name(), definition->types().front()->type());
            }

            for (const auto &type : definition->types()) {
                Asn1Acn::Types::Type *const typeType = type->type();
                if (typeType == nullptr) {
                    continue;
                }
                output.insert(type->name(), typeType);
            }
        }
    }

    return output;
}

int DataReconstructor::getSizeOfType(const Asn1Acn::Types::Type::ASN1Type &type)
{
    switch (type) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER:
        return sizeOfInteger;
    case Asn1Acn::Types::Type::ASN1Type::REAL:
        return sizeOfReal;
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN:
        return sizeOfBoolean;
    default:
        return 0;
    }
}

QByteArray DataReconstructor::popFrontQByteArray(int howMany, QByteArray &array)
{
    QByteArray output = array.left(howMany);
    array.remove(0, howMany);
    return output;
}

} // namespace testgenerator
