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

#pragma once

#include <asn1library/asn1/asn1model.h>
#include <ivcore/ivmodel.h>
#include <map>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <seds/SedsModel/components/states/entrystate.h>
#include <seds/SedsModel/components/states/exitstate.h>
#include <seds/SedsModel/sedsmodel.h>

namespace conversion::sdl::translator {

class AssignmentInfo
{
public:
    AssignmentInfo(QString left, QString right);

    auto left() const -> QString;
    auto right() const -> QString;

private:
    QString m_left;
    QString m_right;
};

class ActivityInfo
{
public:
    auto name() -> QString;
    auto returnAssignments() const -> const std::vector<AssignmentInfo> &;
    ActivityInfo() = default;
    ActivityInfo(QString name);
    auto addAssignment(AssignmentInfo assignment) -> void;

private:
    QString m_name;
    std::vector<AssignmentInfo> m_returnAssignments;
};

/**
 *  @brief  Translation context
 */
class Context
{
public:
    /**
     * @brief   Context constructor
     *
     * @param sedsPackage       Source SEDS Package
     * @param sedsComponent     Source SEDS Component
     * @param asn1Model         ASN.1 Model
     * @param ivfunction        IV Function
     * @param sdlProcess        Target SDL Process
     * @param sdlStateMachine   Target SDL State Machine
     */
    Context(const seds::model::Package &sedsPackage, const seds::model::Component &sedsComponent,
            Asn1Acn::Asn1Model *asn1Model, ivm::IVFunction *ivFunction, ::sdl::Process *sdlProcess,
            ::sdl::StateMachine *sdlStateMachine);

    /**
     * @brief SEDS Package accessor
     *
     * @returns SEDS Package
     */

    auto sedsPackage() -> const seds::model::Package &;
    /**
     * @brief Component accessor
     *
     * @returns Component
     */
    auto sedsComponent() -> const seds::model::Component &;
    /**
     * @brief IV Function accessor
     *
     * @returns IV Function
     */
    auto ivFunction() -> ivm::IVFunction *;
    /**
     * @brief ASN.1 Model accessor
     *
     * @returns ASN.1 Model
     */
    auto asn1Model() -> Asn1Acn::Asn1Model *;
    /**
     * @brief SDL Process accessor
     *
     * @returns SDL Process
     */
    auto sdlProcess() -> ::sdl::Process *;
    /**
     * @brief SDL State Machine accessor
     *
     * @returns SDL State Machine
     */
    auto sdlStateMachine() -> ::sdl::StateMachine *;

    auto addCommand(const QString interface, const QString name, const seds::model::InterfaceCommand *definition)
            -> void;

    auto getCommand(const QString interface, const QString name) -> const seds::model::InterfaceCommand *;

    auto commands() -> const std::vector<std::pair<QString, const seds::model::InterfaceCommand *>>;

    auto addActivityInfo(const QString name, ActivityInfo info) -> void;

    auto getActivityInfo(QString name) -> const ActivityInfo *;

private:
    const seds::model::Package &m_sedsPackage;
    const seds::model::Component &m_sedsComponent;
    Asn1Acn::Asn1Model *m_asn1Model;
    ivm::IVFunction *m_ivFunction;
    ::sdl::Process *m_sdlProcess;
    ::sdl::StateMachine *m_sdlStateMachine;
    std::map<std::pair<QString, QString>, const seds::model::InterfaceCommand *> m_commands;
    std::map<QString, ActivityInfo> m_activityInfos;
};

}
