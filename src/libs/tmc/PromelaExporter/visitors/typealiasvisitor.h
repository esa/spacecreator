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
#include <tmc/PromelaModel/basictypes.h>
#include <tmc/PromelaModel/typealias.h>
#include <tmc/PromelaModel/utyperef.h>

namespace conversion::tmc::exporter {
/**
 * @brief Visitor for TypeAlias. Generates string representation.
 */
class TypeAliasVisitor final
{
public:
    /**
     * @brief Constructor.
     *
     * @param stream text stream to output string representation of TypeAlias
     */
    TypeAliasVisitor(QTextStream &stream);

    /**
     * @brief operator for TypeAlias
     *
     * Generates string representation of TypeAlias
     *
     * @param typeAlias value of TypeAlias
     */
    void operator()(const ::tmc::promela::model::TypeAlias &typeAlias);

    /**
     * @brief operator for BasicType
     *
     * Generates string representation of BasicType
     *
     * @param basicType value of BasicType
     */
    void operator()(const ::tmc::promela::model::BasicType &basicType);
    /**
     * @brief operator for UtypeRef
     *
     * Generates string representation of UtypeRef
     *
     * @param utypeRef value of UtypeRef
     */
    void operator()(const ::tmc::promela::model::UtypeRef &utypeRef);

private:
    QTextStream &m_stream;
};
}
