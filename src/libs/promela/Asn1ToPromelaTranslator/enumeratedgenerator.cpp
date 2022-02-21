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

#include "enumeratedgenerator.h"

#include <algorithm>
#include <conversion/common/escaper/escaper.h>

using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using conversion::Escaper;

namespace promela::translator {
EnumeratedGenerator::EnumeratedGenerator(QString typeName, const Enumerated &enumerated)
    : m_typeName(typeName)
{
    m_items = enumerated.items().values().toVector();
    std::sort(m_items.begin(), m_items.end(),
            [](const EnumeratedItem &lhs, const EnumeratedItem &rhs) { return lhs.index() < rhs.index(); });

    m_iter = m_items.begin();
}

void EnumeratedGenerator::reset()
{
    m_iter = m_items.begin();
}

std::pair<QString, int32_t> EnumeratedGenerator::next()
{
    QString name = QString("%1_%2").arg(m_typeName).arg(Escaper::escapePromelaName(m_iter->name()));
    int32_t value = m_iter->value();

    ++m_iter;

    return std::make_pair(name, value);
}

bool EnumeratedGenerator::has_next() const
{
    return m_iter != m_items.end();
}

}
