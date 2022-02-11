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
 * @brief Generator that generates all possible integer values from integer subset.
 */
class EnumeratedGenerator final
{
public:
    EnumeratedGenerator(QString typeName, const ::Asn1Acn::Types::Enumerated &enumerated);
    void reset();
    std::pair<QString, int32_t> next();
    bool has_next() const;

private:
    const QString m_typeName;
    QVector<::Asn1Acn::Types::EnumeratedItem> m_items;
    QVector<::Asn1Acn::Types::EnumeratedItem>::const_iterator m_iter;
};
}
