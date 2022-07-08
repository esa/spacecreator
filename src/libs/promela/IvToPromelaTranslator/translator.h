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

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/definitions.h>
#include <conversion/common/translation/translator.h>
#include <ivcore/ivinterface.h>
#include <promela/PromelaModel/promelamodel.h>
#include <shared/veobject.h>

namespace promela::translator {
/**
 * @brief Translate between InterfaceView model and Promela Model
 */
class IvToPromelaTranslator final : public ::conversion::translator::Translator
{
private:
    /**
     * @brief Parsed Observer Attachment specification
     */
    class ObserverAttachment
    {
    public:
        /**
         * @brief Kind of the attachment
         */
        enum class Kind
        {
            Kind_Input, //< Input signal interception, after processing by the recipient
            Kind_Output, //< Ouput signal interception, before processing by the recipient
        };

        /**
         * @brief Observer priority
         */
        using Priority = uint32_t;

        /**
         * @brief Constructor
         *
         * @param specification Textual Observer Attachment specification to be parsed
         */
        ObserverAttachment(const QString &specification);

        /**
         * @brief Getter for the signal recipient function
         *
         * @return Optional name of the recipient function
         */
        auto toFunction() const -> std::optional<QString>;
        /**
         * @brief Getter for the signal sender function
         *
         * @return Optional name of the sender function
         */
        auto fromFunction() const -> std::optional<QString>;
        /**
         * @brief Getter for the name of the interface (sender's or recipient's, depending on the kind)
         *
         * @return Interface (signal) name
         */
        auto interface() const -> const QString &;
        /**
         * @brief Getter for the observer name
         *
         * @return Observer name
         */
        auto observer() const -> const QString &;
        /**
         * @brief Getter for the observer's interface name
         *
         * @return Observer inteface (signal) name
         */
        auto observerInterface() const -> const QString &;

        /**
         * @brief Getter for the observer's kind (input or output)
         *
         * @return Observer kind
         */
        auto kind() const -> Kind;

        /**
         * @brief Getter for the observer's priority (lower number is higher priority)
         *
         * @return Observer priority
         */
        auto priority() const -> Priority;

    private:
        std::optional<QString> m_fromFunctionName;
        std::optional<QString> m_toFunctionName;
        QString m_interfaceName;
        QString m_observerName;
        QString m_observerInterfaceName;
        Priority m_priority;
        Kind m_kind;

        static auto stringToKind(const QString &kind) -> Kind;
    };

    /**
     * @brief Aggregation of multiple Observer Attachments
     */
    using ObserverAttachments = std::vector<ObserverAttachment>;

    /**
     * @brief Translation context
     */
    class Context
    {
    public:
        /**
         * @brief Constructor
         *
         * @param promelaModel The model that is being created
         */
        Context(model::PromelaModel *promelaModel);

        /**
         * @brief Add observer attachment
         *
         * @param attachment Attachment to be added
         */
        auto addObserverAttachment(const ObserverAttachment &attachment) -> void;

        /**
         * @brief Get attachments of the given kind applicable to the given interface of the given function
         *
         * @param function Name of the function
         * @param interface Name of the function's interface
         * @param kind Kind of the attachments to be retrieved
         *
         * @return List of found attachments
         */
        auto getObserverAttachments(const QString &function, const QString &interface,
                const ObserverAttachment::Kind kind) -> const ObserverAttachments;

        /**
         * @brief Return whether there are attachment of the given kind applicable to the given interface of the given
         * function
         *
         * @param function Name of the function
         * @param interface Name of the function's interface
         * @param kind Kind of the attachments to be retrieved
         *
         * @return Whether the specified attachments exist
         */
        auto hasObserverAttachments(
                const QString &function, const QString &interface, const ObserverAttachment::Kind kind) -> bool;

        /**
         * Getter for the Promela model that is being created
         *
         * @return Promela model
         */
        auto model() -> model::PromelaModel *;

    private:
        model::PromelaModel *m_promelaModel;
        std::map<QString, std::map<QString, ObserverAttachments>> m_observerAttachments;
    };

public:
    /**
     * @brief   Translate given InterfaceView model into an Promela model
     *
     * @param   sourceModels    List of models
     * @param   options         List of options
     *
     * @return  Promela model translated from the InterfaceView model
     */
    auto translateModels(std::vector<conversion::Model *> sourceModels, const conversion::Options &options) const
            -> std::vector<std::unique_ptr<conversion::Model>> override;

    /**
     * @brief   Return a model type that is a source of this translator
     *
     * @return  Source model type
     */
    auto getSourceModelType() const -> conversion::ModelType override;
    /**
     * @brief   Returns a model type that is a target of this translator
     *
     * @return  Target model type
     */
    auto getTargetModelType() const -> conversion::ModelType override;
    /**
     * @brief   Provides a set of all source model types that are required for the translation
     *
     * @return  Set of required models
     */
    auto getDependencies() const -> std::set<conversion::ModelType> override;

private:
    static const char *TIMER_MANAGER_DATA_NAME;
    static const char *TIMER_MANAGER_PROCTYPE_NAME;

private:
    auto addChannelAndLock(IvToPromelaTranslator::Context &context, const QString &functionName) const -> void;
    auto observerInputSignalName(const IvToPromelaTranslator::ObserverAttachment &attachment) const -> QString;
    auto attachInputObservers(IvToPromelaTranslator::Context &context, const QString &functionName,
            const QString &interfaceName, const QString &parameterName, const QString &parameterType,
            promela::model::Sequence *sequence) const -> void;
    auto generateInitProctype(const std::vector<QString> &modelFunctions, const std::vector<QString> &observers,
            const ::ivm::IVModel *ivModel) const -> model::InitProctype;
    auto generateProctype(Context &context, const QString &functionName, const QString &interfaceName,
            const QString &parameterType, size_t queueSize, size_t priority, bool environment) const
            -> std::unique_ptr<model::Proctype>;
    auto generateEnvironmentProctype(const QString &functionName, const QString &interfaceName,
            const QString &parameterType, const QString &sendInline, const conversion::Options &options) const
            -> std::unique_ptr<model::Proctype>;
    auto generateSendInline(const QString &functionName, const QString &interfaceName, const QString &parameterName,
            const QString &parameterType, const QString &sourceFunctionName, const QString &sourceInterfaceName) const
            -> std::unique_ptr<model::InlineDef>;
    auto createPromelaObjectsForFunction(Context &context, const ::ivm::IVModel *ivModel, ::ivm::IVFunction *ivFunction,
            const QString &functionName) const -> void;
    auto createPromelaObjectsForEnvironment(Context &context, const ::ivm::IVModel *ivModel,
            ::ivm::IVFunction *ivFunction, const QString &functionName,
            const std::vector<const Asn1Acn::Definitions *> &asn1SubtypesDefinitions,
            const conversion::Options &options) const -> void;
    auto createCheckQueueInline(
            model::PromelaModel *promelaModel, const QString &functionName, QList<QString> &channelNames) const -> void;
    auto createSystemState(model::PromelaModel *promelaModel, const ::ivm::IVModel *ivModel,
            const std::vector<QString> &modelFunctions, const std::vector<QString> &observers) const -> void;
    auto createPromelaObjectsForTimers(
            Context &context, const ::ivm::IVModel *ivModel, const std::vector<QString> &modelFunctions) const -> void;
    auto createTimerInlinesForFunction(
            Context &context, const QString &functionName, const QString &timerName, int timerId) const -> void;
    auto createGlobalTimerObjects(Context &context, int timerCount, const std::map<int, QString> &timerSignals) const
            -> void;
    auto createWaitForInitStatement() const -> std::unique_ptr<model::ProctypeElement>;

    auto containsContextVariables(const QVector<shared::ContextParameter> &parameters) const -> bool;
    auto constructChannelName(const QString &functionName, const QString &interfaceName) const -> QString;

    auto getSubtypesDefinitions(const Asn1Acn::Asn1Model *asn1Model, const conversion::Options &options) const
            -> std::vector<const Asn1Acn::Definitions *>;
    auto getInterfaceName(const ivm::IVInterface *interface) const -> QString;
    auto getInterfaceFunctionName(const ivm::IVInterface *interface) const -> QString;
    auto getInterfaceProperty(ivm::IVInterface *interface, const QString &name) const -> QVariant;
    auto getInterfaceParameter(const ivm::IVInterface *interface) const -> std::pair<QString, QString>;
    auto getInterfaceQueueSize(ivm::IVInterface *interface) const -> size_t;
    auto getInterfacePriority(ivm::IVInterface *interface) const -> size_t;

    auto handleParameterType(const QString &parameterTypeName, const QString &parameterName,
            const QString &interfaceName, const QString &functionName,
            const std::vector<const Asn1Acn::Definitions *> &asn1SubtypesDefinitions) const -> QString;
    auto buildParameterSubtypeName(
            const QString &functionName, const QString &interfaceName, const QString &parameterName) const -> QString;
};
}
