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

#include <QStringList>
#include <memory>
#include <seds/SedsModel/sedsmodel.h>

namespace seds::model {
class ContainerDataType;
} // namespace seds::model

namespace tests::conversion::common {

class SedsModelBuilder final
{
public:
    SedsModelBuilder(QString name);
    std::unique_ptr<seds::model::SedsModel> build();

public:
    auto withArrayDataType(QString name, QString itemTypeName, uint32_t dimensionsCount) -> SedsModelBuilder &;
    auto withBinaryDataType(QString name) -> SedsModelBuilder &;
    auto withBooleanDataType(QString name) -> SedsModelBuilder &;
    auto withContainerDataType(seds::model::ContainerDataType dataType) -> SedsModelBuilder &;
    auto withEnumeratedDataType(QString name, QStringList elems) -> SedsModelBuilder &;
    auto withFloatDataType(QString name) -> SedsModelBuilder &;
    auto withIntegerDataType(QString name) -> SedsModelBuilder &;
    auto withStringDataType(QString name) -> SedsModelBuilder &;

    auto withComponent(seds::model::Component component) -> SedsModelBuilder &;

private:
    seds::model::Package m_package;
};

} // namespace tests::conversion::common
