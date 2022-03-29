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

#include <QString>
#include <QVariant>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>

namespace testgenerator {

class DataReconstructor final
{
public:
    static auto getVariantVectorFromRawData(QByteArray rawData, unsigned int TestVectorsNumber, ivm::IVInterface *iface,
            Asn1Acn::Asn1Model *asn1Model) -> QVector<QVariant>;

private:
    // these values shall be configurable by a user
    static const int sizeOfInteger = 8;
    static const int sizeOfReal = 8;
    static const int sizeOfBoolean = 8;

    static auto mapAsnTypesToPlatformTypes(Asn1Acn::Asn1Model *model) -> QMap<QString, Asn1Acn::Types::Type *>;
    static auto getSizeOfType(const Asn1Acn::Types::Type::ASN1Type &type) -> int;
    static auto popFrontQByteArray(int howMany, QByteArray &array) -> QByteArray;

    template<typename T>
    static void pushBackCopyToVariantVector(QVector<QVariant> &vector, QByteArray srcData)
    {
        const T *const var = reinterpret_cast<const T *>(srcData.data());
        vector.push_back(QVariant(*var));
    }
};

} // namespace testgenerator
