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

#include <asn1library/asn1/definitions.h>

namespace seds::model {
class ArrayDataType;
class BinaryDataType;
class BooleanDataType;
class ContainerDataType;
class EnumeratedDataType;
class FloatDataType;
class IntegerDataType;
class StringDataType;
class SubRangeDataType;
} // namespace seds::model

namespace conversion::asn1::translator {

struct DataTypeTranslatorVisitor final {
    Asn1Acn::Definitions *m_definitions;

    void operator()(const seds::model::ArrayDataType &dataType);
    void operator()(const seds::model::BinaryDataType &dataType);
    void operator()(const seds::model::BooleanDataType &dataType);
    void operator()(const seds::model::ContainerDataType &dataType);
    void operator()(const seds::model::EnumeratedDataType &dataType);
    void operator()(const seds::model::FloatDataType &dataType);
    void operator()(const seds::model::IntegerDataType &dataType);
    void operator()(const seds::model::StringDataType &dataType);
    void operator()(const seds::model::SubRangeDataType &dataType);
};

} // namespace conversion::asn1::translator
