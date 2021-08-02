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

class QXmlStreamAttribute;
class QXmlStreamReader;

namespace seds::model {
class GenericAlternate;
class GenericAlternateSet;
class GenericType;
class GenericTypeMap;
class GenericTypeMapSet;
class InterfaceDeclaration;
} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for generics
 */
class GenericsParser final
{
  public:
    /**
     * @brief   Adds all generic types to given interface declaration
     * @param   interfaceDeclaration    Interface declaration to which constructed  will be added
     */
    static auto readGenericTypeSet(model::InterfaceDeclaration& interfaceDeclaration, QXmlStreamReader& xmlReader)
            -> void;
    /**
     * @brief   Constructs GenericTypeMapSet object
     * @return  GenericTypeMapSet
     */
    static auto readGenericTypeMapSet(QXmlStreamReader& xmlReader) -> model::GenericTypeMapSet;

  private:
    /**
     * @brief   Constructs GenericType object
     * @return  GenericType
     */
    static auto readGenericType(QXmlStreamReader& xmlReader) -> model::GenericType;

    /**
     * @brief   Constructs GenericTypeMap object
     * @return  GenericTypeMap
     */
    static auto readGenericTypeMap(QXmlStreamReader& xmlReader) -> model::GenericTypeMap;
    /**
     * @brief   Constructs AlternateSet object
     * @return  AlternateSet
     */
    static auto readGenericAlternateSet(QXmlStreamReader& xmlReader) -> model::GenericAlternateSet;
    /**
     * @brief   Constructs Alternate object
     * @return  Alternate
     */
    static auto readGenericAlternate(QXmlStreamReader& xmlReader) -> model::GenericAlternate;
};

} // namespace seds::parser
