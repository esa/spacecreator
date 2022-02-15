/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "statemachinetranslator.h"
#include "statementtranslatorvisitor.h"

#include <algorithm>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/SedsToIvTranslator/specialized/componentstranslator.h>
#include <conversion/iv/SedsToIvTranslator/specialized/interfacecommandtranslator.h>
#include <conversion/iv/SedsToIvTranslator/specialized/interfaceparametertranslator.h>
#include <conversion/iv/SedsToIvTranslator/translator.h>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/task.h>

using conversion::Escaper;
using conversion::asn1::translator::SedsToAsn1Translator;
using conversion::iv::translator::ComponentsTranslator;
using conversion::iv::translator::InterfaceCommandTranslator;
using conversion::iv::translator::InterfaceParameterTranslator;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::MissingInterfaceViewFunctionException;
using conversion::translator::TranslationException;

namespace conversion::sdl::translator {

AssignmentInfo::AssignmentInfo(QString left, QString right)
{
    m_left = left;
    m_right = right;
}

auto AssignmentInfo::left() const -> QString
{
    return m_left;
}
auto AssignmentInfo::right() const -> QString
{
    return m_right;
}

auto ActivityInfo::name() -> QString
{
    return m_name;
}

auto ActivityInfo::returnAssignments() const -> const std::vector<AssignmentInfo> &
{
    return m_returnAssignments;
}

ActivityInfo::ActivityInfo(QString name)
{
    m_name = name;
}
auto ActivityInfo::addAssignment(AssignmentInfo assignment) -> void
{
    m_returnAssignments.push_back(assignment);
}

Context::Context(const seds::model::Package &sedsPackage, const seds::model::Component &sedsComponent,
        Asn1Acn::Asn1Model *asn1Model, ivm::IVFunction *ivFunction, ::sdl::Process *sdlProcess,
        ::sdl::StateMachine *sdlStateMachine)
    : m_sedsPackage(sedsPackage)
    , m_sedsComponent(sedsComponent)
    , m_asn1Model(asn1Model)
    , m_ivFunction(ivFunction)
    , m_sdlProcess(sdlProcess)
    , m_sdlStateMachine(sdlStateMachine)
{
}

auto Context::sedsPackage() -> const seds::model::Package &
{
    return m_sedsPackage;
}

auto Context::sedsComponent() -> const seds::model::Component &
{
    return m_sedsComponent;
}

auto Context::asn1Model() -> Asn1Acn::Asn1Model *
{
    return m_asn1Model;
}

auto Context::ivFunction() -> ivm::IVFunction *
{
    return m_ivFunction;
}

auto Context::sdlProcess() -> ::sdl::Process *
{
    return m_sdlProcess;
}

auto Context::sdlStateMachine() -> ::sdl::StateMachine *
{
    return m_sdlStateMachine;
}

auto Context::addCommand(const QString interface, const QString name, const seds::model::InterfaceCommand *definition)
        -> void
{
    m_commands[std::make_pair(interface, name)] = definition;
}

auto Context::getCommand(const QString interface, const QString name) -> const seds::model::InterfaceCommand *
{
    const auto i = m_commands.find(std::make_pair(interface, name));
    if (i == m_commands.end()) {
        return nullptr;
    }
    return i->second;
}

auto Context::commands() -> const std::vector<std::pair<QString, const seds::model::InterfaceCommand *>>
{
    std::vector<std::pair<QString, const seds::model::InterfaceCommand *>> result;
    for (const auto &i : m_commands) {
        result.push_back(std::make_pair(i.first.first, i.second));
    }
    return result;
}

auto Context::addActivityInfo(const QString name, ActivityInfo info) -> void
{
    m_activityInfos[name] = info;
}

auto Context::getActivityInfo(QString name) -> const ActivityInfo *
{
    if (m_activityInfos.find(name) != m_activityInfos.end()) {
        return &m_activityInfos[name];
    }
    return nullptr;
}

}