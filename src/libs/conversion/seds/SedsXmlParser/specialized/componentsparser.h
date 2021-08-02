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

#include <memory>

#include <seds/SedsCommon/basetypesmappings.h>

class QXmlStreamAttribute;
class QXmlStreamReader;

namespace seds::model {
class Component;
class ComponentImplementation;
class Interface;
class Package;
class ParameterActivityMap;
class ParameterMap;
class ParameterMapData;
class Variable;
class VariableRef;

} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for components
 */
class ComponentsParser final
{
  public:
    /**
     * @brief   Adds all components to given package
     * @param   package     Package to which constructed Components will be added
     */
    static auto readComponentSet(model::Package& package, QXmlStreamReader& xmlReader) -> void;

    /**
     * @brief   Constructs VariableRef object
     * @return  VariableRef
     */
    static auto readVariableRef(QXmlStreamReader& xmlReader) -> model::VariableRef;

  private:
    /**
     * @brief   Constructs Component object
     * @return  Component
     */
    static auto readComponent(QXmlStreamReader& xmlReader) -> model::Component;
    /**
     * @brief   Constructs ComponentImplementation object
     * @return  ComponentImplementation
     */
    static auto readComponentImplementation(QXmlStreamReader& xmlReader) -> model::ComponentImplementation;

    /**
     * @brief   Adds all provided interfaces to given component
     * @param   package     Component to which constructed Interfaces will be added
     */
    static auto readProvidedInterfaceSet(model::Component& component, QXmlStreamReader& xmlReader) -> void;
    /**
     * @brief   Adds all provided interfaces to given component
     * @param   package     Component to which constructed Interfaces will be added
     */
    static auto readRequiredInterfaceSet(model::Component& component, QXmlStreamReader& xmlReader) -> void;
    /**
     * @brief   Constructs Interface object
     * @return  Interface
     */
    static auto readInterface(QXmlStreamReader& xmlReader) -> model::Interface;

    /**
     * @brief   Adds all variables to given component implementation
     * @param   implementation    Component implementation to which constructed Variables will be added
     */
    static auto readVariableSet(model::ComponentImplementation& implementation, QXmlStreamReader& xmlReader) -> void;
    /**
     * @brief   Constructs Variable object
     * @return  Variable
     */
    static auto readVariable(QXmlStreamReader& xmlReader) -> model::Variable;

    /**
     * @brief   Adds all parameter maps to given component implementation
     * @param   implementation    Component implementation to which constructed ParameterMaps will be added
     */
    static auto readParameterMapSet(model::ComponentImplementation& implementation, QXmlStreamReader& xmlReader)
            -> void;
    /**
     * @brief   Constructs ParameterMap object
     * @return  ParameterMap
     */
    static auto readParameterMap(QXmlStreamReader& xmlReader) -> model::ParameterMap;

    /**
     * @brief   Adds all parameter activity maps to given component implementation
     * @param   implementation    Component implementation to which constructed ParameterActivityMaps will be added
     */
    static auto readParameterActivityMapSet(model::ComponentImplementation& implementation, QXmlStreamReader& xmlReader)
            -> void;
    /**
     * @brief   Constructs ParameterActivityMap object
     * @return  ParameterActivityMap
     */
    static auto readParameterActivityMap(QXmlStreamReader& xmlReader) -> model::ParameterActivityMap;
    /**
     * @brief   Constructs ParameterMapData object
     * @return  ParameterMapData
     */
    static auto readParameterMapData(QXmlStreamReader& xmlReader) -> model::ParameterMapData;
};

} // namespace seds::parser
