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

#include "specialized/componentstatesparser.h"

#include <QXmlStreamReader>

#include <seds/SedsModel/components/componentimplementation.h>
#include <seds/SedsModel/components/states/entrystate.h>
#include <seds/SedsModel/components/states/exitstate.h>
#include <seds/SedsModel/components/states/state.h>
#include <seds/SedsModel/components/states/statemachine.h>
#include <seds/SedsModel/components/states/statemachineelement.h>
#include <seds/SedsModel/components/states/transition.h>

#include "exceptions.h"
#include "specialized/componentactivitiesparser.h"
#include "specialized/componentprimitivesparser.h"
#include "specialized/coreparser.h"

namespace seds::parser {

void
ComponentStatesParser::readStateMachineSet(model::ComponentImplementation& implementation, QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("StateMachine")) {
            implementation.addStateMachine(readStateMachine(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "StateMachineSet");
        }
    }
}

model::StateMachine
ComponentStatesParser::readStateMachine(QXmlStreamReader& xmlReader)
{
    model::StateMachine stateMachine;

    for(const auto& attribute : xmlReader.attributes()) {
        if(CoreParser::processForNamedEntity(&stateMachine, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("defaultEntryState")) {
            stateMachine.setDefaultEntryState(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(CoreParser::processForNamedEntity(&stateMachine, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("EntryState")) {
            stateMachine.addElement(readEntryState(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("ExitState")) {
            stateMachine.addElement(readExitState(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("State")) {
            stateMachine.addElement(readState(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Transition")) {
            stateMachine.addElement(readTransition(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "StateMachine");
        }
    }

    return stateMachine;
}

model::EntryState
ComponentStatesParser::readEntryState(QXmlStreamReader& xmlReader)
{
    model::EntryState entryState;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStateMachineElement(&entryState, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStateMachineElement(&entryState, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "EntryState");
        }
    }

    return entryState;
}

model::ExitState
ComponentStatesParser::readExitState(QXmlStreamReader& xmlReader)
{
    model::ExitState exitState;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStateMachineElement(&exitState, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStateMachineElement(&exitState, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "ExitState");
        }
    }

    return exitState;
}

model::State
ComponentStatesParser::readState(QXmlStreamReader& xmlReader)
{
    model::State state;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStateMachineElement(&state, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStateMachineElement(&state, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("OnEntry")) {
            state.setOnEntry(ComponentActivitiesParser::readActivityInvocation(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("OnExit")) {
            state.setOnExit(ComponentActivitiesParser::readActivityInvocation(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "State");
        }
    }

    return state;
}

model::Transition
ComponentStatesParser::readTransition(QXmlStreamReader& xmlReader)
{
    model::Transition transition;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStateMachineElement(&transition, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("fromState")) {
            transition.setFromState(attribute.value().toString());
        } else if(attribute.name() == QStringLiteral("toState")) {
            transition.setToState(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStateMachineElement(&transition, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("OnCommandPrimitive")) {
            transition.setPrimitive(ComponentPrimitivesParser::readOnCommandPrimitive(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("OnParameterPrimitive")) {
            transition.setPrimitive(ComponentPrimitivesParser::readOnParameterPrimitive(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("OnTimer")) {
            transition.setPrimitive(ComponentPrimitivesParser::readTimerSink(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Guard")) {
            transition.setGuard(ComponentActivitiesParser::readBooleanExpression(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Do")) {
            transition.setDoActivity(ComponentActivitiesParser::readActivityInvocation(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Transition");
        }
    }

    return transition;
}

bool
ComponentStatesParser::processForStateMachineElement(model::StateMachineElement* object,
                                                     const QXmlStreamAttribute& attribute)
{
    return CoreParser::processForNamedEntity(object, attribute);
}

bool
ComponentStatesParser::processForStateMachineElement(model::StateMachineElement* object, QXmlStreamReader& xmlReader)
{
    return CoreParser::processForNamedEntity(object, xmlReader);
}

} // namespace seds::parser
