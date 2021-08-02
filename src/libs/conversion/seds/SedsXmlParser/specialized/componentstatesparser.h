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

class QXmlStreamAttribute;
class QXmlStreamReader;

namespace seds::model {
class ComponentImplementation;
class EntryState;
class ExitState;
class State;
class StateMachine;
class StateMachineElement;
class Transition;
} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for component states
 */
class ComponentStatesParser final
{
public:
    /**
     * @brief   Adds all parameter maps to given component implementation
     * @param   implementation    Component implementation to which constructed StateMachines will be added
     */
    static auto readStateMachineSet(model::ComponentImplementation &implementation, QXmlStreamReader &xmlReader)
            -> void;

private:
    /**
     * @brief   Constructs StateMachine object
     * @return  StateMachine
     */
    static auto readStateMachine(QXmlStreamReader &xmlReader) -> model::StateMachine;
    /**
     * @brief   Constructs EntryState object
     * @return  EntryState
     */
    static auto readEntryState(QXmlStreamReader &xmlReader) -> model::EntryState;
    /**
     * @brief   Constructs ExitState object
     * @return  ExitState
     */
    static auto readExitState(QXmlStreamReader &xmlReader) -> model::ExitState;
    /**
     * @brief   Constructs State object
     * @return  State
     */
    static auto readState(QXmlStreamReader &xmlReader) -> model::State;
    /**
     * @brief   Constructs Transition object
     * @return  Transition
     */
    static auto readTransition(QXmlStreamReader &xmlReader) -> model::Transition;

    /**
     * @brief   Processes attribute for StateMachine
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForStateMachineElement(model::StateMachineElement *object, const QXmlStreamAttribute &attribute)
            -> bool;
    /**
     * @brief   Processes element for StateMachine
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForStateMachineElement(model::StateMachineElement *object, QXmlStreamReader &xmlReader) -> bool;
};

} // namespace seds::parser
