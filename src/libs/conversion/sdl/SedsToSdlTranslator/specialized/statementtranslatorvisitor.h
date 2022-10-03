/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "common.h"
#include "components/activities/polynomial.h"
#include "components/activities/splinecalibrator.h"

#include <asn1library/asn1/asn1model.h>
#include <ivcore/ivmodel.h>
#include <optional>
#include <sdl/SdlModel/decision.h>
#include <sdl/SdlModel/join.h>
#include <sdl/SdlModel/label.h>
#include <sdl/SdlModel/output.h>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/task.h>
#include <seds/SedsModel/sedsmodel.h>

namespace conversion {
class Options;
} // namespace conversion

namespace conversion::sdl::translator {

/**
 * @brief Forward declaration of an internal class used to reduce glue code
 *
 * For friendship declaration
 */
class ExpressionTranslatorVisitor;

/**
 * @brief   Translator visitor for SEDS statements
 *
 * Translated statements will be added to the passed transition
 */
class StatementTranslatorVisitor final
{
public:
    /**
     *  @brief  Statement translation context which can be shared between visitors
     */
    class StatementContext
    {
    public:
        /**
         * @brief   StatementContext constructor
         *
         * @param masterContext     Master context of the translation
         * @param sdlProcess        Host SDL Process
         * @param sdlProcedure      Host SDL Procedure
         */
        StatementContext(Context &masterContext, ::sdl::Process *sdlProcess, ::sdl::Procedure *sdlProcedure);

        /**
         * @brief   Returns a unique label name, starting with the given prefix
         *
         * @param   prefix   Label name prefix
         * @returns Unique label name
         */
        auto uniqueLabelName(const QString &prefix = "label_") -> QString;
        /**
         * @brief   Returns a unique raw spline points array name
         *
         * @returns Unique array name
         */
        auto uniqueRawSplinePointsVariableName() -> QString;
        /**
         * @brief   Returns a unique calibrated spline points array name
         *
         * @returns Unique array name
         */
        auto uniqueCalibratedSplinePointsVariableName() -> QString;

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
         * @brief InterfaceView Function accessor
         *
         * @returns InterfaceView Function Package
         */
        auto ivFunction() -> ivm::IVFunction *;
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

        /**
         * @brief   Check if interval index variable was already created for current procedure
         *
         * @param   True if variable was created, false otherwise
         */
        auto intervalIndexVariableCreated() const -> bool;
        /**
         * @brief   Set creation status of the interval index variable
         *
         * @param   created     Interval index variable creation status
         */
        auto setIntervalIndexVariableCreated(bool created) -> void;

        /**
         * @brief   Set new count as max if bigger than current max
         *
         * @param   count   Point count
         */
        auto handleSplinePointCount(const std::size_t count) -> void;

        /**
         * @brief Add ActivityInfo
         *
         * @param name  Name of the Activity
         * @param info  ActivityInfo to be added
         */
        auto addActivityInfo(const QString &name, ActivityInfo info) -> void;

        /**
         * @brief Retrieve InterfaceCommand
         *
         * @param interface     Name of the interface hosting the command
         * @param name          Name of the command
         *
         * @return Command info
         */
        auto getCommand(const QString &interface, const QString &name) -> const CommandInfo *;

    private:
        int m_labelCount;
        int m_rawSplinePointsVariableCount;
        int m_calibratedSplinePointsVariableCount;
        Context &m_masterContext;
        ::sdl::Process *m_sdlProcess;
        ::sdl::Procedure *m_sdlProcedure;
        bool m_intervalIndexVariableCreated;
    };

    /**
     * @brief Visitor constructor
     *
     * @param context           Shared translation context
     * @param sdlTransition     Target SDL transition
     */
    StatementTranslatorVisitor(StatementContext &context, ::sdl::Transition *sdlTransition, const Options &options);

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
     * @brief   Translates SEDS sendParameter
     *
     * @param   sendParameter   Statement to translate
     */
    auto operator()(const seds::model::SendParameterPrimitive &sendParameter) -> void;

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

    /**
     * @brief   Translates SEDS BooleanExpression into SDL decision
     *
     * @param sdlProcess        Host SDL process
     * @param sdlProcedure      Host SDL procedure
     * @param expression        Expression to be translated
     *
     * @returns SDL decision
     */
    static auto translateBooleanExpression(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::BooleanExpression &expression) -> std::unique_ptr<::sdl::Decision>;

    /**
     * @brief Translate SEDS variable reference to an SDL one
     *
     * @param reference Variable reference to be translated
     *
     * @returns Translated variable reference
     */
    static auto translateVariableReference(const QString &reference) -> QString;

private:
    StatementContext &m_context;
    ::sdl::Transition *m_sdlTransition;
    const Options &m_options;

    static const inline QString m_sedsTransactionParamName = "sedsTransactionName";

    static auto findIvInterface(ivm::IVFunction *function, const QString &interfaceName) -> ivm::IVInterface *;

    static auto translateArgument(const seds::model::NamedArgumentValue &argument) -> ::sdl::ProcedureCall::Argument;

    static auto translatePolynomial(const QString variable, const seds::model::Polynomial &polynomial) -> QString;

    static auto translateCall(::sdl::Process *hostProcess, const QString callName,
            const seds::model::SendCommandPrimitive &sendCommand, ivm::IVInterface *ivInterface, const Options &options)
            -> std::unique_ptr<::sdl::ProcedureCall>;

    static auto translateCall(::sdl::Process *hostProcess, const QString callName,
            const seds::model::SendParameterPrimitive &sendParameter) -> std::unique_ptr<::sdl::ProcedureCall>;

    static auto translateOutput(const QString &callName, const seds::model::SendCommandPrimitive &sendCommand,
            const Options &options) -> std::vector<std::unique_ptr<::sdl::Action>>;

    static auto translateOutput(const QString &callName, const seds::model::SendParameterPrimitive &sendParameter)
            -> std::vector<std::unique_ptr<::sdl::Action>>;

    static auto translateComparison(const seds::model::Comparison &comparison) -> QString;

    static auto translateAndedConditions(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::AndedConditions &conditions) -> QString;

    static auto translateOredConditions(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::OredConditions &conditions) -> QString;

    static auto translateTypeCheck(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
            const seds::model::TypeCheck &check) -> QString;

    static auto translateAnswer(StatementContext &context, ::sdl::Label *joinLabel, const QString &value,
            const seds::model::Body *body, const Options &options) -> std::unique_ptr<::sdl::Answer>;

    static auto comparisonOperatorToString(const seds::model::ComparisonOperator op) -> QString;

    static auto getOperandValue(const seds::model::Operand &operand) -> QString;

    static auto translateBody(StatementContext &context, ::sdl::Transition *transition, const seds::model::Body *body,
            const Options &options) -> void;

    auto generateSplineCalibratorBoilerplate(const seds::model::SplineCalibrator &splineCalibrator) const -> void;

    static auto generateLoopStart(
            ::sdl::Transition *transition, const seds::model::Iteration &iteration, ::sdl::Decision *decision) -> void;

    static auto generateLoopEnd(
            ::sdl::Transition *transition, const seds::model::Iteration &iteration, ::sdl::Label *startLabel) -> void;

    static auto handleTransaction(const ::seds::model::Name &transaction, ::sdl::ProcedureCall *call,
            ivm::IVInterface *ivInterface, const Options &options) -> void;

    friend class ExpressionTranslatorVisitor;
};

} // namespace conversion::asn1::translator
