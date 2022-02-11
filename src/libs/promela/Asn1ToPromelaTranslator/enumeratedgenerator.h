/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * Thips library is free software; you can redistribute it and/or
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

#include <QString>
#include <QVector>
#include <asn1library/asn1/types/enumerated.h>
#include <cstdint>
#include <utility>

namespace promela::translator {
/**
 * @brief Generator that generates all possible values of enumerated ASN1 data type.
 *
 * Generator generates names and values for all items of enumeration.
 * The name of enumerated data type is used as a prefix for all items.
 */
class EnumeratedGenerator final
{
public:
    /**
     * @brief Constructor.
     *
     * @param typeName name of enum type.
     * @param enumerated ASN.1 Enumerated data type.
     */
    EnumeratedGenerator(QString typeName, const ::Asn1Acn::Types::Enumerated &enumerated);

    /**
     * @brief Reset the generator to initial state.
     */
    void reset();
    /**
     * @brief Get the current item and generate the next one.
     *
     * @return Current item as pair, where first is name of item and the second is value.
     */
    std::pair<QString, int32_t> next();
    /**
     * @brief Check if generator is able to generate next value.
     *
     * @return true if generator can generate next value, otherwise false
     */
    bool has_next() const;

private:
    const QString m_typeName;
    QVector<::Asn1Acn::Types::EnumeratedItem> m_items;
    QVector<::Asn1Acn::Types::EnumeratedItem>::const_iterator m_iter;
};
}
