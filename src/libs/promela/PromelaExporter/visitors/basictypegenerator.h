/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <QTextStream>
#include <promela/PromelaModel/basictypes.h>

namespace promela::exporter {
/**
 * @brief Generate string representation of BasicType
 */
class BasicTypeGenerator final
{
public:
    /**
     * @brief Constructor.
     *
     * Construct element with output stream.
     *
     * @param stream text stream to output string representation of BasicType
     */
    BasicTypeGenerator(QTextStream &stream);

    /**
     * @brief generate string representation of basic type
     *
     * @param type value of BasicType, which will be converted to string
     */
    void generate(::promela::model::BasicType type);

private:
    QTextStream &m_stream;
};
}
