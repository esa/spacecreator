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

#include "visitors/datatypetranslatorvisitor.h"

#include "types/arraydatatype.h"
#include "types/binarydatatype.h"
#include "types/booleandatatype.h"
#include "types/containerdatatype.h"
#include "types/enumerateddatatype.h"
#include "types/floatdatatype.h"
#include "types/integerdatatype.h"
#include "types/stringdatatype.h"
#include "types/subrangedatatype.h"

#include <QDebug>

using seds::model::ArrayDataType;
using seds::model::BinaryDataType;
using seds::model::BooleanDataType;
using seds::model::ContainerDataType;
using seds::model::EnumeratedDataType;
using seds::model::FloatDataType;
using seds::model::IntegerDataType;
using seds::model::StringDataType;
using seds::model::SubRangeDataType;

namespace conversion::asn1::translator {

void DataTypeTranslatorVisitor::operator()(const ArrayDataType &dataType)
{
    qDebug() << "array";
}

void DataTypeTranslatorVisitor::operator()(const BinaryDataType &dataType)
{
    qDebug() << "binary";
}

void DataTypeTranslatorVisitor::operator()(const BooleanDataType &dataType)
{
    qDebug() << "bool";
}

void DataTypeTranslatorVisitor::operator()(const ContainerDataType &dataType)
{
    qDebug() << "container";
}

void DataTypeTranslatorVisitor::operator()(const EnumeratedDataType &dataType)
{
    qDebug() << "enum";
}

void DataTypeTranslatorVisitor::operator()(const FloatDataType &dataType)
{
    qDebug() << "float";
}

void DataTypeTranslatorVisitor::operator()(const IntegerDataType &dataType)
{
    qDebug() << "integer";
}

void DataTypeTranslatorVisitor::operator()(const StringDataType &dataType)
{
    qDebug() << "string";
}

void DataTypeTranslatorVisitor::operator()(const SubRangeDataType &dataType)
{
    qDebug() << "subrange";
}

} // namespace conversion::asn1::translator
