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

DataReconstructor::TypeLayoutInfos::TypeLayoutInfos(std::initializer_list<TypeLayoutInfo> infos)
{
    for (const auto &info : infos) {
        this->insert(std::get<0>(info), QPair<TypeDataLength, TypePaddingLength>(std::get<1>(info), std::get<2>(info)));
    }
}

QVector<QVariant> DataReconstructor::getVariantVectorFromRawData(QByteArray rawData,
        const unsigned int numberOfTestVectors, ivm::IVInterface *const iface, Asn1Acn::Asn1Model *const asn1Model,
        QDataStream::ByteOrder endianness, const TypeLayoutInfos &typeLayoutInfos)
{
    QVector<QVariant> output;
    output.reserve(static_cast<int>(numberOfTestVectors));

    const auto definitionNameToTypeMap = mapDefinitionNameToType(asn1Model);

    for (unsigned int i = 0; i < numberOfTestVectors; i++) {
        for (const auto &param : iface->params()) {
            const auto &type = definitionNameToTypeMap.value(param.paramTypeName());
            if (type == nullptr) {
                throw std::runtime_error("Requested type cannot be found");
            }

            const int dataLength = typeLayoutInfos.value(type->typeName()).first;
            const int paddingLength = typeLayoutInfos.value(type->typeName()).second;
            const int variableLength = dataLength + paddingLength;
            QByteArray rawVariable = popFrontQByteArray(variableLength, rawData);
            if (endianness == QDataStream::BigEndian) {
                std::reverse(rawVariable.begin(), rawVariable.end());
            }

            const auto typeEnum = type->typeEnum();
            if (typeEnum == Asn1Acn::Types::Type::ASN1Type::INTEGER) {
                pushBackCopyToVariantVector<int>(output, rawVariable);
            } else if (typeEnum == Asn1Acn::Types::Type::ASN1Type::REAL) {
                pushBackCopyToVariantVector<double>(output, rawVariable);
            } else if (typeEnum == Asn1Acn::Types::Type::ASN1Type::BOOLEAN) {
                pushBackCopyToVariantVector<bool>(output, rawVariable);
            }
        }
    }

    return output;
}

QMap<QString, Asn1Acn::Types::Type *> DataReconstructor::mapDefinitionNameToType(Asn1Acn::Asn1Model *const model)
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
            if (definition->typeAssignmentNames().isEmpty()) {
                continue;
            }

            for (const auto &definitionType : definition->types()) {
                Asn1Acn::Types::Type *const type = definitionType->type();
                const QString &name = definitionType->name();

                if (type == nullptr || name.isEmpty()) {
                    continue;
                }

                output.insert(name, type);
            }
        }
    }

    return output;
}

QByteArray DataReconstructor::popFrontQByteArray(const int howMany, QByteArray &array)
{
    QByteArray output = array.left(howMany);
    array.remove(0, howMany);

    return output;
}

} // namespace testgenerator
