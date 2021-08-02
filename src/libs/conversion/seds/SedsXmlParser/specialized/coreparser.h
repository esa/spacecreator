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

#include <seds/SedsCommon/basetypesmappings.h>
#include <seds/SedsModel/base/positivelong.h>

class QXmlStreamAttribute;
class QXmlStreamReader;

namespace seds::model {
class DataTypeRef;
class Description;
class ExternalField;
class Field;
class NamedEntity;
} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for core elements
 */
class CoreParser final
{
  public:
    /**
     * @brief   Reads current element as a long description
     * @return  Long description
     */
    static auto readLongDescription(QXmlStreamReader& xmlReader) -> common::String;

    /**
     * @brief   Parses given string as a boolean
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseBool(QStringRef valueStr) -> bool;
    /**
     * @brief   Parses given string as a 8-bit unsigned integer
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseUInt8(QStringRef valueStr) -> uint8_t;
    /**
     * @brief   Parses given string as a 32-bit integer
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseInt32(QStringRef valueStr) -> int32_t;
    /**
     * @brief   Parses given string as a 64-bit integer
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseInt64(QStringRef valueStr) -> int64_t;
    /**
     * @brief   Parses given string as a 64-bit unsigned integer
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseUInt64(QStringRef valueStr) -> uint64_t;
    /**
     * @brief   Parses given string as a 64-bit floating point
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseDouble(QStringRef valueStr) -> double;
    /**
     * @brief   Parses given string as a PositiveLong
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parsePositiveLong(QStringRef valueStr) -> model::PositiveLong::Value;

    /**
     * @brief   Processes attribute for Description
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForDescription(model::Description* object, const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for Description
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForDescription(model::Description* object, QXmlStreamReader& xmlReader) -> bool;
    /**
     * @brief   Processes attribute for NamedEntity
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForNamedEntity(model::NamedEntity* object, const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for NamedEntity
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForNamedEntity(model::NamedEntity* object, QXmlStreamReader& xmlReader) -> bool;
};

} // namespace seds::parser
