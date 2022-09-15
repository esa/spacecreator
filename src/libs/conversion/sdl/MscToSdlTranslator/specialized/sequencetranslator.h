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

#include "mscparametervalueparser.h"
#include "signalinfo.h"
#include "tftable.h"

#include <QStringList>
#include <asn1library/asn1/file.h>
#include <conversion/common/options.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <memory>
#include <msccore/mscmessage.h>
#include <sdl/SdlModel/decision.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <vector>

namespace conversion::sdl::translator {

class SequenceTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel    Parent SDL model
     * @param   asn1File    ASN.1 with observer dataview
     * @param   ivModel     IV model
     * @param   options     Conversion options
     */
    SequenceTranslator(::sdl::SdlModel *sdlModel, const Asn1Acn::File *asn1File, const ivm::IVModel *ivModel,
            const Options &options);

    /**
     * @brief   Deleted copy constuctor
     */
    SequenceTranslator(const SequenceTranslator &) = delete;
    /**
     * @brief   Deleted move constuctor
     */
    SequenceTranslator(SequenceTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    SequenceTranslator &operator=(const SequenceTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    SequenceTranslator &operator=(SequenceTranslator &&) = delete;

protected:
    struct Context {
        QString chartName;
        std::vector<std::unique_ptr<::sdl::Rename>> signalRenames;
        std::unordered_map<uint32_t, SignalInfo> signals;
        std::size_t signalCounter;
    };

protected:
    using SignalsMap = std::unordered_map<uint32_t, SignalInfo>;
    using StateList = std::vector<std::unique_ptr<::sdl::State>>;
    using ActionsMap = std::unordered_map<const ::sdl::Rename *, std::vector<std::unique_ptr<::sdl::Action>>>;
    using TransitionList = std::vector<std::unique_ptr<::sdl::Transition>>;

    auto getRenamedSignal(Context &context, const msc::MscMessage *mscMessage) const -> ::sdl::Rename *;
    auto renameSignal(const QString &name, const msc::MscMessage *mscMessage) const -> std::unique_ptr<::sdl::Rename>;

    auto getSequenceValue(Context &context, const ::sdl::Rename *renamedSignal, const msc::MscMessage *mscMessage) const
            -> uint32_t;

    auto createSdlProcess(const QString &chartName, std::unique_ptr<::sdl::StateMachine> stateMachine)
            -> ::sdl::Process;
    auto createSdlSystem(const QString &chartName, ::sdl::Process process) -> ::sdl::System;

    auto createStates(const uint32_t stateCount) const -> StateList;
    auto createTransitions(const TFTable &table, StateList &states, const SignalsMap &signals,
            const MscParameterValueParser::SignalRequirementsMap &signalRequirements, const uint32_t startStateId) const
            -> TransitionList;

    auto createSignalRequirements(const MscParameterValueParser::ParametersRequirements &parametersRequirements,
            const ::sdl::State *targetState) const -> std::unique_ptr<::sdl::Action>;
    auto createParameterRequirements(const QString &name, const std::optional<QString> &value,
            std::unique_ptr<::sdl::Action> trueAction) const -> std::unique_ptr<::sdl::Decision>;
    auto createTrueAnswer(std::unique_ptr<::sdl::Action> action, const QString &literal) const
            -> std::unique_ptr<::sdl::Answer>;
    auto createElseAnswer() const -> std::unique_ptr<::sdl::Answer>;

    auto createTransitionOnSignal(const QString &signalName, const int parameterCount, ::sdl::State *sourceState) const
            -> std::unique_ptr<::sdl::Transition>;

    auto getArgumentsTypes(const QString &ivFunctionName, const QString &ivInterfaceName) const -> QStringList;
    auto findIvInterface(const QString &ivFunctionName, const QString &ivInterfaceName) const -> ivm::IVInterface *;

private:
    auto createSignalActions(const uint32_t startStateId, const std::vector<uint32_t> &transitions, StateList &states,
            const SignalsMap &signals, const MscParameterValueParser::SignalRequirementsMap &signalRequirements) const
            -> ActionsMap;

    auto splitChoiceName(const QString &name) const -> std::pair<QString, QString>;

protected:
    inline static const QString m_observerNameThen = "then";
    inline static const QString m_observerNameThenNot = "then not";
    inline static const QString m_stateNameTemplate = "s%1";
    inline static const QString m_signalRenameNameTemplate = "sig%1";
    inline static const QString m_signalParameterNameTemplate = "%1_param%2";
    inline static const QString m_errorStateName = "sErr";
    inline static const QString m_okStateName = "sOk";
    inline static const QString m_anySignalName = "*";
    inline static const QString m_defaultChannelName = "c";
    inline static const QString m_defaultRouteName = "r";
    inline static const QString m_isPresentTemplate = "present(%1)";
    inline static const QString m_elseLiteral = "ELSE";

    ::sdl::SdlModel *m_sdlModel;
    const Asn1Acn::File *m_asn1File;
    const ivm::IVModel *m_ivModel;
    const Options &m_options;
};

} // namespace conversion::sdl::translator
