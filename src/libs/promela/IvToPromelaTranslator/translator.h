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
        std::optional<QString> m_parameterName;

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
        Context(model::PromelaModel *promelaModel, const ivm::IVModel *ivModel, const conversion::Options &options,
                const std::vector<const Asn1Acn::Definitions *> &asn1SubtypesDefinitons,
                const std::vector<QString> &modelFunctions, const std::vector<QString> &observerNames);

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

        auto getObserverAttachments(const ObserverAttachment::Kind kind) -> const ObserverAttachments;

        /**
         * Getter for the Promela model that is being created
         *
         * @return Promela model
         */
        auto model() const -> model::PromelaModel *;

        /**
         * Getter for the IV model that is translated
         *
         * @return IV model
         */
        auto ivModel() const -> const ivm::IVModel *;

        /**
         * Getter for the conversion options
         *
         * @return conversion options
         */
        auto options() const -> const conversion::Options &;

        /**
         * Getter for the ASN.1 Subtypes Definitions
         *
         * @return conversion options
         */
        auto subtypesDefinitions() const -> const std::vector<const Asn1Acn::Definitions *> &;

        /**
         * Getter for model function list
         *
         * @return list of model functions
         */
        auto modelFunctions() const -> const std::vector<QString> &;

        /**
         * Getter for list of observers
         *
         * @return list of names of observers
         */
        auto observerNames() const -> const std::vector<QString> &;

        /**
         * @brief Setter for base priority for proctypes
         *
         * @param priority new base proctype priority
         */
        auto setBaseProctypePriority(uint32_t priority) -> void;

        /**
         * @brief Getter base priority for proctypes
         *
         * @return base priority for proctypes
         */
        auto getBaseProctypePriority() const -> uint32_t;

    private:
        model::PromelaModel *m_promelaModel;
        const ivm::IVModel *m_ivModel;
        const conversion::Options &m_options;
        const std::vector<const Asn1Acn::Definitions *> &m_asn1SubtypesDefinitons;
        const std::vector<QString> &m_modelFunctions;
        const std::vector<QString> &m_observerNames;
        uint32_t m_baseProctypePriority;
        std::map<QString, std::map<QString, ObserverAttachments>> m_fromObserverAttachments;
        std::map<QString, std::map<QString, ObserverAttachments>> m_toObserverAttachments;
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
    inline static const QString m_timerManagerDataName = "timer_manager_data";
    inline static const QString m_timerManagerProctypeName = "timer_manager_proc";
    inline static const QString m_dummyParamName = "dummy";
    inline static const QString m_systemInitedVariableName = "inited";

private:
    auto initializeFunction(model::Sequence &sequence, const QString &functionName) const -> void;
    auto addChannelAndLock(IvToPromelaTranslator::Context &context, const QString &functionName) const -> void;
    auto observerInputSignalName(const IvToPromelaTranslator::ObserverAttachment &attachment) const -> QString;
    auto attachInputObservers(IvToPromelaTranslator::Context &context, const QString &functionName,
            const QString &interfaceName, const QString &parameterName, const QString &parameterType) const
            -> std::list<std::unique_ptr<promela::model::ProctypeElement>>;
    auto generateInitProctype(Context &context) const -> void;
    auto generateProctype(Context &context, const QString &functionName, const QString &interfaceName,
            const QString &parameterType, size_t queueSize, size_t priority, bool environment) const -> void;
    auto generateProcessMessageBlock(const QString &functionName, const QString &channelName, const QString &inlineName,
            const QString &parameterType, const QString &parameterName, const QString &exitLabel, bool lock,
            std::list<std::unique_ptr<promela::model::ProctypeElement>> additionalElements) const
            -> std::unique_ptr<model::ProctypeElement>;
    auto generateEnvironmentProctype(Context &context, const QString &functionName, const QString &interfaceName,
            const QString &parameterType, const QString &sendInline) const -> void;
    auto generateSendInline(Context &context, const QString &functionName, const QString &interfaceName,
            const QString &parameterName, const QString &parameterType, const QString &sourceFunctionName,
            const QString &sourceInterfaceName, const bool parameterSubtyped) const -> void;
    auto createPromelaObjectsForFunction(
            Context &context, const ::ivm::IVFunction *ivFunction, const QString &functionName) const -> void;
    auto createPromelaObjectsForAsyncPis(Context &context, const ivm::IVInterface *providedInterface,
            const QString &functionName, const QString &interfaceName, const std::size_t priority) const -> void;
    auto createPromelaObjectsForSyncRis(
            Context &context, const ivm::IVInterface *requiredInterface, const QString &functionName) const -> void;
    auto createPromelaObjectsForEnvironment(
            Context &context, const ivm::IVFunction *ivFunction, const QString &functionName) const -> void;
    auto createCheckQueueInline(model::PromelaModel *promelaModel, const QString &functionName,
            const QList<QString> &channelNames) const -> void;
    auto createCheckQueuesExpression(const QList<QString> &channelNames, bool empty) const
            -> std::unique_ptr<::promela::model::Expression>;
    auto createSystemState(Context &context) const -> void;
    auto createPromelaObjectsForTimers(Context &context) const -> void;
    auto createTimerInlinesForFunction(
            Context &context, const QString &functionName, const QString &timerName, int timerId) const -> void;
    auto createGlobalTimerObjects(Context &context, int timerCount, const std::map<int, QString> &timerSignals) const
            -> void;
    auto createWaitForInitStatement() const -> std::unique_ptr<model::ProctypeElement>;
    auto createPromelaObjectsForObservers(Context &context) const -> void;

    auto constructChannelName(const QString &functionName, const QString &interfaceName) const -> QString;

    auto getSubtypesDefinitions(const Asn1Acn::Asn1Model *asn1Model, const conversion::Options &options) const
            -> std::vector<const Asn1Acn::Definitions *>;
    auto getInterfaceName(const ivm::IVInterface *interface) const -> QString;
    auto getInterfaceFunctionName(const ivm::IVInterface *interface) const -> QString;
    auto getInterfaceProperty(const ivm::IVInterface *interface, const QString &name) const -> QVariant;
    auto getInterfaceParameter(const ivm::IVInterface *interface) const -> std::pair<QString, QString>;
    auto getInterfaceQueueSize(const ivm::IVInterface *interface) const -> size_t;
    auto getInterfacePriority(const ivm::IVInterface *interface) const -> size_t;

    auto handleParameterSubtype(Context &context, const QString &parameterTypeName, const QString &parameterName,
            const QString &interfaceName, const QString &functionName) const -> QString;
    auto handleSendInlineParameter(const QString &argumentName, const QString &parameterType,
            promela::model::Sequence &sequence) const -> promela::model::Expression;
    auto handleSendInlineArgument(const QString &parameterType, const QString &functionName,
            const QString &interfaceName, const QString parameterName, promela::model::Sequence &sequence) const
            -> QString;

    auto isParameterSubtyped(Context &context, const QString &parameterTypeName, const QString &parameterName,
            const QString &interfaceName, const QString &functionName) const -> bool;

    auto buildParameterSubtypeName(
            const QString &functionName, const QString &interfaceName, const QString &parameterName) const -> QString;

    auto findProvidedInterface(const ivm::IVModel *model, const QString &fromFunction,
            const QString &interfaceName) const -> const ivm::IVInterface *;
    auto findRequiredInterface(const ivm::IVModel *model, const QString &functionName,
            const QString &interfaceName) const -> const ivm::IVInterface *;
    auto findTimerSignal(const ivm::IVModel *model, const QString &functionName, const QString &signalName) const
            -> QString;

    auto observerChannelName(const ObserverAttachment &attachment, const QString &toFunction) const -> QString;
    auto getAttachmentToFunction(const ivm::IVModel *model, const ObserverAttachment &attachment) const -> QString;
    auto getAttachmentFromFunction(const ivm::IVModel *model, const ObserverAttachment &attachment) const -> QString;

    auto getFunctionLockChannelName(const QString &functionName) const -> QString;
    auto createLockAcquireStatement(const QString &functionName) const -> std::unique_ptr<model::ProctypeElement>;
    auto createLockReleaseStatement(const QString &functionName) const -> std::unique_ptr<model::ProctypeElement>;
    auto createProcessInlineCall(const QString &inlineName, const QString &parameterType,
            const QString &parameterName) const -> std::unique_ptr<model::ProctypeElement>;
    auto createReceiveStatement(const QString &channelName, const QString &parameterType,
            const QString &parameterName) const -> std::unique_ptr<model::ProctypeElement>;
    auto getObserverAttachments(Context &context, const QString &function, const QString &interface,
            const ObserverAttachment::Kind kind) const -> ObserverAttachments;
};
}
