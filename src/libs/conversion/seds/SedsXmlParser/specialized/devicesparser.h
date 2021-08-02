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

#include <functional>

#include <seds/SedsCommon/basetypesmappings.h>

#include "device/metadata/metadatavalueset.h"

class QXmlStreamAttribute;
class QXmlStreamReader;

namespace seds::model {
class DateMetadataValue;
class Device;
class FloatMetadataValue;
class IntegerMetadataValue;
class Metadata;
class MetadataCategory;
class MetadataValue;
class StringMetadataValue;
} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for devices
 */
class DevicesParser final
{
  public:
    using MetadataValueAddingFunction = std::function<void(model::MetadataValueType)>;

  public:
    /**
     * @brief   Constructs Device object
     * @param   xmlReader   XML reader
     * @return  Device
     */
    static auto readDevice(QXmlStreamReader& xmlReader) -> model::Device;
    /**
     * @brief   Constructs Metadata object
     * @param   xmlReader   XML reader
     * @return  Metadata
     */
    static auto readMetadata(QXmlStreamReader& xmlReader) -> model::Metadata;

  private:
    /**
     * @brief   Adds all metadata values to given metadata
     * @param   metadata    Metadata declaration to which constructed MetadataValues will be added
     */
    static auto readMetadataValueSet(MetadataValueAddingFunction addValue, QXmlStreamReader& xmlReader) -> void;
    /**
     * @brief   Constructs DateMetadataValue object
     * @param   xmlReader   XML reader
     * @return  DateMetadataValue
     */
    static auto readDateMetadataValue(QXmlStreamReader& xmlReader) -> model::DateMetadataValue;
    /**
     * @brief   Constructs FloatMetadataValue object
     * @param   xmlReader   XML reader
     * @return  FloatMetadataValue
     */
    static auto readFloatMetadataValue(QXmlStreamReader& xmlReader) -> model::FloatMetadataValue;
    /**
     * @brief   Constructs IntegerMetadataValue object
     * @param   xmlReader   XML reader
     * @return  IntegerMetadataValue
     */
    static auto readIntegerMetadataValue(QXmlStreamReader& xmlReader) -> model::IntegerMetadataValue;
    /**
     * @brief   Constructs StringMetadataValue object
     * @param   xmlReader   XML reader
     * @return  StringMetadataValue
     */
    static auto readStringMetadataValue(QXmlStreamReader& xmlReader) -> model::StringMetadataValue;

    /**
     * @brief   Constructs MetadataCategory object
     * @param   xmlReader   XML reader
     * @return  MetadataCategory
     */
    static auto readMetadataCategory(QXmlStreamReader& xmlReader) -> model::MetadataCategory;

    /**
     * @brief   Processes attribute for MetadataValue
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForMetadataValue(model::MetadataValue* object, const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for MetadataValue
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForMetadataValue(model::MetadataValue* object, QXmlStreamReader& xmlReader) -> bool;
};

} // namespace seds::parser
