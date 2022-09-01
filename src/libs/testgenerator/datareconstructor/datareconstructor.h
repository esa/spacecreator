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
#include <QMap>
#include <QPair>
#include <QString>
#include <QVariant>
#ifndef Q_OS_MACOS
#include <endian.h>
#endif
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <qglobal.h>

namespace testgenerator {

/**
 * @brief Class for reconstructing values according to specified information
 *
 */
class DataReconstructor final
{
public:
    typedef int TypePaddingLength; // bytes
    typedef int TypeDataLength; // bytes
    typedef QString TypeName;

    typedef std::tuple<TypeName, TypeDataLength, TypePaddingLength> TypeLayoutInfo;

    /**
     * @brief Container to store user-supplied information about datatypes to be reconstructed
     *
     */
    class TypeLayoutInfos final : public QMap<TypeName, QPair<TypeDataLength, TypePaddingLength>>
    {
    public:
        /**
         * @brief Default constructor of the class
         *
         */
        TypeLayoutInfos() = default;
        /**
         * @brief Construct a new object storing user-supplied information about datatypes
         *
         * @param infos list of information about datatypes
         */
        TypeLayoutInfos(std::initializer_list<TypeLayoutInfo> infos);
    };

    /**
     * @brief Get the vector of variants of values from raw data array, based on information about tested interface and
     *        data format
     *
     * @param rawData              raw data bytes array
     * @param iface                interface under test
     * @param asn1Model            ASN.1 model describing required types
     * @param endianness           big endian or little endian
     * @param typeLayoutInfos      map of length and padding of data on a remote target
     *
     * @return vector of variants of values read from raw data
     */
    static auto getVariantVectorFromRawData(const QByteArray &rawData, ivm::IVInterface *iface,
            Asn1Acn::Asn1Model *asn1Model, QDataStream::ByteOrder endianness = QDataStream::LittleEndian,
            const TypeLayoutInfos &typeLayoutInfos = {
                    { "INTEGER", 8, 0 },
                    { "BOOLEAN", 1, 7 },
                    { "REAL", 8, 0 },
            }) -> QVector<QVariant>;

private:
    static auto mapDefinitionNameToType(Asn1Acn::Asn1Model *model) -> QMap<QString, Asn1Acn::Types::Type *>;

    template<typename T>
    static void pushBackCopyToVariantVector(QVector<QVariant> &vector, QByteArray srcData)
    {
        const T *const var = reinterpret_cast<const T *>(srcData.data());
        vector.push_back(QVariant(*var));
    }
};

} // namespace testgenerator
