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

#include "base/description.h"
#include "base/qualifiedname.h"
#include "components/component.h"
#include "interfaces/interfacedeclaration.h"
#include "types/arraydatatype.h"
#include "types/binarydatatype.h"
#include "types/booleandatatype.h"
#include "types/containerdatatype.h"
#include "types/datatype.h"
#include "types/enumerateddatatype.h"
#include "types/floatdatatype.h"
#include "types/integerdatatype.h"
#include "types/rootdatatype.h"
#include "types/stringdatatype.h"
#include "types/subrangedatatype.h"
#include "xincludeinfo.h"

#include <memory>
#include <seds/SedsCommon/basetypesmappings.h>

namespace seds::model {

class Package final : public Description
{
public:
    using ComponentSet = std::vector<Component>;
    using DataTypeSet = std::vector<DataType>;
    using DeclaredInterfaceSet = std::vector<InterfaceDeclaration>;

public:
    Package() = default;
    Package(Package &&) = default;
    Package &operator=(Package &&) = default;

public:
    auto qualifiedName() const -> const QualifiedName &;
    auto setName(common::String name) -> void;

    auto xIncludes() const -> const std::vector<XIncludeInfo> &;
    auto addXInclude(XIncludeInfo info) -> void;

    auto dataType(const common::String &name) const -> const DataType *;
    auto dataTypes() const -> const DataTypeSet &;
    auto addDataType(DataType dataType) -> void;

    auto declaredInterfaces() const -> const DeclaredInterfaceSet &;
    auto addInterfaceDeclaration(InterfaceDeclaration interfaceDeclaration) -> void;

    auto components() const -> const ComponentSet &;
    auto addComponent(Component component) -> void;

public:
    /**
     * @brief   Get name string
     *
     * @return Name string
     */
    auto nameStr() const -> const QString &;

private:
    QualifiedName m_qualifiedName;

    std::vector<XIncludeInfo> m_xIncludes;

    DataTypeSet m_dataTypes;
    DeclaredInterfaceSet m_declaredInterfaces;
    ComponentSet m_components;
};

} // namespace seds::model
