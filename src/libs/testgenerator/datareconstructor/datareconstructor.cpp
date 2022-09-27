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

#include <QDebug>

namespace testgenerator {

DataReconstructor::TypeLayoutInfos::TypeLayoutInfos(std::initializer_list<TypeLayoutInfo> infos)
{
    for (const auto &info : infos) {
        this->insert(std::get<0>(info), QPair<TypeDataLength, TypePaddingLength>(std::get<1>(info), std::get<2>(info)));
    }
}

int findNextMultipleOf(int number, int i)
{
    if (i % number != 0) {
        return i + number - i % number;
    } else {
        return i;
    }
}

QVector<QVariant> DataReconstructor::getVariantVectorFromRawData(const QByteArray &rawData,
        ivm::IVInterface *const iface, Asn1Acn::Asn1Model *const asn1Model, QDataStream::ByteOrder endianness,
        const TypeLayoutInfos &typeLayoutInfos)
{
    QVector<QVariant> output;
    output.reserve(rawData.size());

    const auto definitionNameToTypeMap = mapDefinitionNameToType(asn1Model);
    constexpr int DATA_ROW_ALINGMENT = 8;

    int i = 0;
    int rowSize = 0;
    int rowCounter = 0;
    while (i < rawData.size()) {
        for (const auto &param : iface->params()) {
            const auto &type = definitionNameToTypeMap.value(param.paramTypeName());
            if (type == nullptr) {
                throw std::runtime_error("Requested type cannot be found");
            }

            const int dataLength = typeLayoutInfos.value(type->typeName()).first;
            i = findNextMultipleOf(dataLength, i);

            QByteArray rawVariable = rawData.mid(i, dataLength);
            i += dataLength;

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
        if (rowCounter == 0) {
            rowSize = i;
        }
        int nextMultiple = findNextMultipleOf(DATA_ROW_ALINGMENT, i);
        if (i + rowSize > nextMultiple) {
            i = nextMultiple;
        }
        rowCounter++;
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

} // namespace testgenerator
