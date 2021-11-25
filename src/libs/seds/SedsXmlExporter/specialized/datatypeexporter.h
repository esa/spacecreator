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

#include <QDomElement>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/binarydatatype.h>
#include <seds/SedsModel/types/booleandatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/datatype.h>
#include <seds/SedsModel/types/encodings/coreintegerencoding.h>
#include <seds/SedsModel/types/encodings/falsevalue.h>
#include <seds/SedsModel/types/enumerateddatatype.h>
#include <seds/SedsModel/types/floatdatatype.h>
#include <seds/SedsModel/types/integerdatatype.h>
#include <seds/SedsModel/types/ranges/minmaxrange.h>
#include <seds/SedsModel/types/stringdatatype.h>
#include <seds/SedsModel/types/subrangedatatype.h>

namespace seds::exporter {

/**
 * @brief   Exporter for SEDS data types
 */
class DataTypeExporter final
{
public:
    /**
     * @brief   Export SEDS interface
     *
     * @param   dataType        Data type to export
     * @param   setElement      Parent set element
     * @param   sedsDocument    SEDS XML document
     */
    static auto exportDataType(const model::DataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument)
            -> void;

protected:
    static auto exportArrayDataType(
            const model::ArrayDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;
    static auto exportBinaryDataType(
            const model::BinaryDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;
    static auto exportBooleanDataType(
            const model::BooleanDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;
    static auto exportContainerDataType(
            const model::ContainerDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;
    static auto exportEnumeratedDataType(
            const model::EnumeratedDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;
    static auto exportFloatDataType(
            const model::FloatDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;
    static auto exportIntegerDataType(
            const model::IntegerDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;
    static auto exportStringDataType(
            const model::StringDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;
    static auto exportSubRangeDataType(
            const model::SubRangeDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void;

    static auto exportEnumerationList(const std::vector<model::ValueEnumeration> &values, QDomElement &setElement,
            QDomDocument &sedsDocument) -> void;
    static auto exportCoreIntegerEncoding(model::CoreIntegerEncoding encoding, QDomElement &setElement) -> void;
    static auto exportCoreEncodingAndPrecision(model::CoreEncodingAndPrecision encoding, QDomElement &setElement)
            -> void;
    static auto exportMinMaxRange(const model::MinMaxRange &range, QDomElement &setElement, QDomDocument &sedsDocument)
            -> void;
    static auto exportFloatPrecisionRange(const model::FloatPrecisionRange &range, QDomElement &setElement) -> void;
};

} // namespace seds::exporter
