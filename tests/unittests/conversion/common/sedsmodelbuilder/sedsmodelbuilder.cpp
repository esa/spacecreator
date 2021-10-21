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

#include "sedsmodelbuilder.h"

#include "sedsdatatypefactory.h"

using namespace seds::model;

namespace tests::conversion::common {

SedsModelBuilder::SedsModelBuilder(QString name)
{
    m_package.setName(std::move(name));
}

std::unique_ptr<SedsModel> SedsModelBuilder::build()
{
    PackageFile packageFile;
    packageFile.setPackage(std::move(m_package));

    return std::make_unique<SedsModel>(std::move(packageFile));
}

SedsModelBuilder &SedsModelBuilder::withArrayDataType(QString name, QString itemTypeName, uint32_t dimensionsCount)
{
    auto dataType = SedsDataTypeFactory::createArray(std::move(name), std::move(itemTypeName), dimensionsCount);
    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withBinaryDataType(QString name)
{
    auto dataType = SedsDataTypeFactory::createBinary(std::move(name));
    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withBooleanDataType(QString name)
{
    auto dataType = SedsDataTypeFactory::createBoolean(std::move(name));
    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withContainerDataType(seds::model::ContainerDataType dataType)
{
    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withEnumeratedDataType(QString name, QStringList elems)
{
    auto dataType = SedsDataTypeFactory::createEnumerated(std::move(name), std::move(elems));
    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withFloatDataType(QString name)
{
    auto dataType = SedsDataTypeFactory::createFloat(std::move(name));
    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withIntegerDataType(QString name)
{
    auto dataType = SedsDataTypeFactory::createInteger(std::move(name));
    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withStringDataType(QString name)
{
    auto dataType = SedsDataTypeFactory::createString(std::move(name));
    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withComponent(Component component)
{
    m_package.addComponent(std::move(component));

    return *this;
}

} // namespace tests::conversion::common
