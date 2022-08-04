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
     * @brief   Deleted constructor
     */
    Escaper() = delete;

    /**
     * @brief   Convert the input name to an id acceptable as an SDL identifier
     *
     * @param   name    input name
     *
     * @returns identifier for InterfaceView
     */
    static auto escapeSdlName(QString name) -> QString;

    /**
     * @brief   Convert the input name to an id acceptable as an SDL variable name
     *
     * @param   name    input name
     *
     * @return  Name for SDL variable
     */
    static auto escapeSdlVariableName(QString name) -> QString;

    /**
     * @brief   Convert the input name to an id acceptable in InterfaceView
     *
     * @param   name    input name
     *
     * @returns identifier for InterfaceView
     */
    static auto escapeIvName(QString name) -> QString;

    /**
     * @brief   Convert the input name to an id acceptable as an ASN.1 type name
     *
     * @param   name    input name
     *
     * @returns identifier for ASN.1 type name
     */
    static auto escapeAsn1TypeName(QString name) -> QString;

    /**
     * @brief   Convert the input name to an id acceptable as an ASN.1 package name
     *
     * @param   name    input name
     *
     * @returns ASN.1 package identifier
     */
    static auto escapeAsn1PackageName(QString name) -> QString;

    /**
     * @brief   Convert the input name to an id acceptable as an ASN.1 sequence field name
     *
     * @param   name    input name
     *
     * @returns identifier for ASN.1 sequence field name
     */
    static auto escapeAsn1FieldName(QString name) -> QString;

    /**
     * @brief   Convert the input type name to an id acceptable as a Promela identifier
     *
     * @param   name    input name
     *
     * @returns identifier for Promela
     */
    static auto escapePromelaName(QString name) -> QString;

    /**
     * @brief   Convert the input name to a Promela field identifier
     *
     * @param   name    input name
     *
     * @returns identifier for Promela
     */
    static auto escapePromelaField(QString name) -> QString;

    /**
     * @brief   Convert the input IV name to an id acceptable as a Promela identifier
     *
     * @param   name    input name
     *
     * @returns identifier for Promela
     */
    static auto escapePromelaIV(QString name) -> QString;

    /**
     * @brief   Convert the input name to an id acceptable in the C code
     *
     * @param   name    input name
     *
     * @returns Escaped name
     */
    static auto escapeCName(QString name) -> QString;

    /**
     * @brief   Convert the input name to an id for ASN.1 sequence in the C code
     *
     * @param   name    input name
     *
     * @returns Escaped name
     */
    static auto escapeCSequenceName(QString name) -> QString;

    /**
     * @brief   Convert the input name to an id for ASN.1 entry in the C code
     *
     * @param   name    input name
     *
     * @returns Escaped name
     */
    static auto escapeCFieldName(QString name) -> QString;

private:
    static auto escapeName(QString &name, const QChar &delimeter) -> void;

    static auto isCharInVector(const QChar &c, const std::vector<QChar> &delimeters) -> bool;

    static auto removeLeadingNonletters(QString &name) -> void;

    static auto removeNonalphanumericCharacters(QString &name, const QChar &delimeter) -> void;

    static auto replaceDelimeters(QString &name, const std::vector<QChar> &srcDelimeters, const QChar &dstDelimeter)
            -> void;
};

} // namespace conversion
