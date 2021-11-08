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

#include "../exceptions.h"

#include <QString>
#include <vector>

namespace conversion {

/**
 * @brief   Escaper
 */
class Escaper final
{
public:
    /**
     * @brief   Constructor
     */
    Escaper() = delete;

    /**
     * @brief   Convert input name to id acceptable in InterfaceView
     *
     * @param   name    input name
     *
     * @returns identifier for InterfaceView
     */
    static auto escapeIvName(QString name) -> QString;

    /**
     * @brief   Convert input name to id acceptable as ASN.1 type name
     *
     * @param   name    input name
     *
     * @returns identifier for ASN.1 type name
     */
    static auto escapeAsn1TypeName(QString name) -> QString;

    /**
     * @brief   Convert input name to id acceptable as ASN.1 field name
     *
     * @param   name    input name
     *
     * @returns identifier for ASN.1 field name
     */
    static auto escapeAsn1FieldName(QString name) -> QString;

private:
    static auto replaceDelimetersWithOne(QString &name, QChar dstDelimeter, const std::vector<QChar> &srcDelimeters)
            -> void;

    static auto isCharInVector(const QChar &c, std::vector<QChar> delimeters) -> bool;

    static auto replaceFirstOccurence(QString &name, QChar before, QChar after) -> void;

    static auto removeLeadingNonletters(QString &name) -> void;
};

} // namespace conversion
