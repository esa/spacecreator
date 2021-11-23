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

#include "components/activities/polynomial.h"
#include "components/activities/splinecalibrator.h"

#include <asn1library/asn1/asn1model.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/decision.h>
#include <sdl/SdlModel/join.h>
#include <sdl/SdlModel/label.h>
#include <sdl/SdlModel/output.h>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/task.h>
#include <seds/SedsModel/sedsmodel.h>

namespace conversion::sdl::translator {

/**
 * @brief   Translator visitor for SEDS statements
 *
 * Translated statements will be added to the passed transition
 */
class StatementTranslatorVisitor final
{
public:
    /**
     *  @brief  Translation context which can be shared between visitors
     */
    class Context
    {
    public:
        /**
         * @brief   Context constructor
         *
         * @param sedsPackage       SEDS package containing the activity
         * @param asn1Model         Data model
         * @param ivModel           IV model
         * @param sdlProcess        Host SDL process
         * @param sdlProcedure      Host SDL procedure
         */
        Context(const seds::model::Package &sedsPackage, Asn1Acn::Asn1Model *asn1Model, ivm::IVModel *ivModel,
                ::sdl::Process *sdlProcess, ::sdl::Procedure *sdlProcedure);

        /**
         * @brief   Returns a unique label name, starting with the given prefix
         *
         * @param   prefix   Label name prefix
         * @returns Unique label name
         */
        auto uniqueLabelName(const QString prefix = "label_") -> QString;

        /**
         * @brief SEDS Package accessor
         *
         * @returns SEDS Package
         */

        auto sedsPackage() -> const seds::model::Package &;
        /**
         * @brief ASN.1 Model accessor
         *
         * @returns ASN.1 Model
         */
        auto asn1Model() -> Asn1Acn::Asn1Model *;
        /**
         * @brief InterfaceView Model accessor
         *
         * @returns InterfaceView Model Package
         */
        auto ivModel() -> ivm::IVModel *;
        /**
         * @brief SDL Process accessor
         *
         * @returns SDL Process
         */
        auto sdlProcess() -> ::sdl::Process *;
        /**
         * @brief SDL Procedure accessor
         *
         * @returns SDL Procedure
         */
        auto sdlProcedure() -> ::sdl::Procedure *;

    private:
        int m_labelCount;
        const seds::model::Package &m_sedsPackage;
        Asn1Acn::Asn1Model *m_asn1Model;
        ivm::IVModel *m_ivModel;
        ::sdl::Process *m_sdlProcess;
        ::sdl::Procedure *m_sdlProcedure;
    };

    /**
     * @brief Visitor constructor
     *
     * @param context           Shared translation context
     * @param sdlTransition     Target SDL transition
     */

    StatementTranslatorVisitor(Context &context, ::sdl::Transition *sdlTransition);

    /**
     * @brief   Translates SEDS activity invocation
     *
     * @param   invocation   Statement to translate
     */
    auto operator()(const seds::model::ActivityInvocation &invocation) -> void;
    /**
     * @brief   Translates SEDS assignment
     *
     * @param   assignment   Statement to translate
     */
    auto operator()(const seds::model::Assignment &assignment) -> void;
    /**
     * @brief   Translates SEDS calibration
     *
     * @param   calibration   Statement to translate
     */
    auto operator()(const seds::model::Calibration &calibration) -> void;
    /**
     * @brief   Translates SEDS conditional
     *
     * @param   conditional   Statement to translate
     */
    auto operator()(const seds::model::Conditional &conditional) -> void;
    /**
     * @brief   Translates SEDS iteration
     *
     * @param   iteration   Statement to translate
     */
    auto operator()(const seds::model::Iteration &iteration) -> void;
    /**
     * @brief   Translates SEDS operation
     *
     * @param   operation   Statement to translate
     */
    auto operator()(const seds::model::MathOperation &operation) -> void;

    /**
     * @brief   Translates SEDS sendCommand
     *
     * @param   sendCommand   Statement to translate
     */
    auto operator()(const seds::model::SendCommandPrimitive &sendCommand) -> void;

    /**
     * @brief   Translates SEDS sendPrimitive
     *
     * @param   sendPrimitive   Statement to translate
     */
    auto operator()(const seds::model::SendParameterPrimitive &sendPrimitive) -> void;

    /**
     * @brief   Translates SEDS activity invocation into SDL task
     *
     * @param   process      Host SDL process
     * @param   invocation   Statement to translate
     *
     * @returns SDL task
     */
    static auto translateActivityCall(::sdl::Process *process, const seds::model::ActivityInvocation &invocation)
            -> std::unique_ptr<::sdl::ProcedureCall>;

private:
    Context &m_context;
    ::sdl::Transition *m_sdlTransition;

    static auto findInterfaceDeclaration(ivm::IVModel *model, const QString functionName, const QString interfaceName)
            -> ivm::IVInterface *;

    static auto findVariableDeclaration(::sdl::Process *process, ::sdl::Procedure *sdlProcedure, QString name)
            -> ::sdl::VariableDeclaration *;

    static auto translateArgument(::sdl::Process *process, ::sdl::Procedure *sdlProcedure,
            const seds::model::NamedArgumentValue &argument) -> ::sdl::ProcedureCall::Argument;

    static auto translatePolynomial(const QString variable, const seds::model::Polynomial &polynomial) -> QString;

    static auto translateCall(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure, const QString callName,
            const seds::model::SendCommandPrimitive &sendCommand) -> std::unique_ptr<::sdl::ProcedureCall>;

    static auto translateOutput(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure, const QString callName,
            const seds::model::SendCommandPrimitive &sendCommand) -> std::vector<std::unique_ptr<::sdl::Action>>;

    static auto translateBooleanExpression(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::BooleanExpression &expression) -> std::unique_ptr<::sdl::Decision>;

    static auto translateComparison(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::Comparison &comparison) -> std::unique_ptr<::sdl::Decision>;

    static auto translateAndedConditions(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::AndedConditions &conditions) -> std::unique_ptr<::sdl::Decision>;

    static auto translateOredConditions(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::OredConditions &conditions) -> std::unique_ptr<::sdl::Decision>;

    static auto translateTypeCheck(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::TypeCheck &check) -> std::unique_ptr<::sdl::Decision>;

    static auto translateAnswer(Context &context, ::sdl::Label *joinLabel, const QString value,
            const seds::model::Body *body) -> std::unique_ptr<::sdl::Answer>;

    static auto comparisonOperatorToString(const seds::model::ComparisonOperator op) -> QString;

    static auto getOperandValue(
            ::sdl::Process *process, ::sdl::Procedure *sdlProcedure, const seds::model::Operand &operand) -> QString;

    static auto translateBody(Context &context, ::sdl::Transition *transition, const seds::model::Body *body) -> void;

    static auto generateLoopStart(Context &context, ::sdl::Transition *transition,
            const seds::model::Iteration &iteration, ::sdl::Decision *decision) -> void;

    static auto generateLoopEnd(Context &context, ::sdl::Transition *transition,
            const seds::model::Iteration &iteration, ::sdl::Label *startLabel) -> void;
};

} // namespace conversion::asn1::translator
