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

#pragma once

#include "asn1model.h"

#include <QDataStream>
#include <QString>
#include <QVariant>
#include <endian.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <qglobal.h>

namespace testgenerator {

class DataReconstructor final
{
public:
    static auto getVariantVectorFromRawData(QByteArray rawData, unsigned int numberOfTestVectors,
            ivm::IVInterface *iface, Asn1Acn::Asn1Model *asn1Model,
            QDataStream::ByteOrder endianness = QDataStream::LittleEndian,
            const QMap<QString, int> &typeSizes = {
                    { "INTEGER", 8 },
                    { "BOOLEAN", 8 },
                    { "REAL", 8 },
            }) -> QVector<QVariant>;

private:
    static auto mapDefinitionNameToType(Asn1Acn::Asn1Model *model) -> QMap<QString, Asn1Acn::Types::Type *>;
    static auto popFrontQByteArray(int howMany, QByteArray &array) -> QByteArray;

    template<typename T>
    static void pushBackCopyToVariantVector(QVector<QVariant> &vector, QByteArray srcData)
    {
        const T *const var = reinterpret_cast<const T *>(srcData.data());
        vector.push_back(QVariant(*var));
    }
};

} // namespace testgenerator
