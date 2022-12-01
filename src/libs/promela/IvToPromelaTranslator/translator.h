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
#include <promela/PromelaModel/promelasystemmodel.h>
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
            Kind_Continuous_Signal, //< Continuous signal in observer
            Kind_Unhandled_Input, //< Unhandled input (signal loss)
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
        Context(model::PromelaSystemModel *promelaModel, const ivm::IVModel *ivModel,
                const conversion::Options &options,
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

        auto getUnhandledInputObserversForFunction(const QString &function, const QString &interface) const
                -> ObserverAttachments;

        /**
         * Getter for the Promela model that is being created
         *
         * @return Promela model
         */
        auto model() const -> model::PromelaSystemModel *;

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
         * @brief Getter for base priority for proctypes
         *
         * @return base priority for proctypes
         */
        auto getBaseProctypePriority() const -> uint32_t;

        /**
         * @brief Getter for vector of observers with continuous signals
         *
         * @return vector of observers with continuous signals
         */
        auto getObserversWithContinuousSignals() const -> const std::vector<QString> &;

    private:
        model::PromelaSystemModel *m_promelaModel;
        const ivm::IVModel *m_ivModel;
        const conversion::Options &m_options;
        const std::vector<const Asn1Acn::Definitions *> &m_asn1SubtypesDefinitons;
        const std::vector<QString> &m_modelFunctions;
        const std::vector<QString> &m_observerNames;
        uint32_t m_baseProctypePriority;
        std::map<QString, std::map<QString, ObserverAttachments>> m_fromObserverAttachments;
        std::map<QString, std::map<QString, ObserverAttachments>> m_toObserverAttachments;
        std::vector<QString> m_observersWithContinuousSignals;
        std::map<QString, std::map<QString, ObserverAttachments>> m_unhandledInputObserverAttachments;
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

    /**
     * @brief Information about output observer
     */
    struct ObserverInfo {
        /**
         * @brief Name of the observer
         */
        QString m_observerName;
        /**
         * @brief Name of the interface to call in the observer
         */
        QString m_observerInterface;
        /**
         * @brief Name of the channel which is owned by observer
         */
        QString m_observerQueue;
    };

    /**
     * @brief Information about proctype in promela
     */
    struct ProctypeInfo {
        /**
         * @brief name of the proctype in promela
         */
        QString m_proctypeName;
        /**
         * @brief name of the interface which proctype originates from
         */
        QString m_interfaceName;
        /**
         * @brief name of the channel in promela used to receive signals
         */
        QString m_queueName;
        /**
         * @brief size of channel in promela
         */
        size_t m_queueSize;
        /**
         * @brief proctype priority
         */
        size_t m_priority;
        /**
         * @brief parameter type name (or empty for parameterless interfaces)
         */
        QString m_parameterTypeName;
        /**
         * @brief parameter name (or empty for parameterless interfaces)
         */
        QString m_parameterName;
        /**
         * @brief true if proctype originates from timer
         */
        bool m_isTimer;

        /**
         * @brief map of possible senders.
         *
         * Currently only one sender is supported.
         * The key is name of IV function
         * The value is name of interface.
         * In case of timer, this is empty container.
         */
        QMap<QString, QString> m_possibleSenders;
        /**
         * @brief List of observers that shall be triggered before processing signal in proctype
         */
        std::list<std::unique_ptr<ObserverInfo>> m_observers;
    };

    /**
     * @brief Information about environment proctype
     */
    struct EnvProctypeInfo {
        /**
         * @brief name of the proctype in promela
         */
        QString m_proctypeName;
        /**
         * @brief name of the interface which proctype originates from
         */
        QString m_interfaceName;
        /**
         * @brief proctype priority
         */
        size_t m_priority;
    };

    /**
     * @brief Information about required synchronous call.
     *
     * This is used to call the target functions interface.
     */
    struct SynchronousCallInfo {
        /**
         * @brief name of the inline
         */
        QString m_name;

        /**
         * @brief name of the inline to call (provided).
         */
        QString m_providedInlineName;
        /**
         * @brief name of the target function
         */
        QString m_targetFunctionName;
        /**
         * @brief true if function requires lock
         */
        bool m_isProtected;

        /**
         * @brief names of the parameters
         */
        QList<QString> m_parameters;
    };

    /**
     * @brief Information about required synchronous call to environment.
     *
     * This is used to generate values for output parameters.
     */
    struct EnvSynchronousCallInfo {
        /**
         * @brief name of the inline
         */
        QString m_name;

        /**
         * @brief Information about parameter.
         */
        struct ParameterInfo {
            /**
             * @brief name of the parameter
             */
            QString m_parameterName;
            /**
             * @brief type of the parameter
             */
            QString m_parameterType;
            /**
             * @brief true if the value shall be generated by env.
             */
            bool m_generateValue;
        };

        /**
         * @brief List of parameters
         */
        QList<ParameterInfo> m_parameters;
    };

    /**
     * @brief Representation of information about IV function in promela system.
     *
     * Every function consists of set proctypes.
     */
    struct FunctionInfo {
        /**
         * @brief The flag to mark IV function as environment
         */
        bool m_isEnvironment;
        /**
         * @brief all model proctypes of function.
         * In case of normal function, only m_proctypes shall be non empty.
         */
        std::map<QString, std::unique_ptr<ProctypeInfo>> m_proctypes;
        /**
         * @brief all environment proctypes, which are used to generate value.
         */
        std::map<QString, std::unique_ptr<EnvProctypeInfo>> m_environmentSourceProctypes;
        /**
         * @brief all environment proctypes, which are used to receive signals from model functions
         */
        std::map<QString, std::unique_ptr<ProctypeInfo>> m_environmentSinkProctypes;

        /**
         * @brief all required synchronous calls to non environment functions.
         */
        std::map<QString, std::unique_ptr<SynchronousCallInfo>> m_synchronousCalls;

        /**
         * @brief all required synchronous calls to environment functions.
         */
        std::map<QString, std::unique_ptr<EnvSynchronousCallInfo>> m_environmentSynchronousCalls;
    };

    /**
     * @brief Representation of structure of promela system.
     */
    struct SystemInfo {
        /**
         * @brief all functions from IV that are part of promela system.
         *
         * This includes environment functions. The key in the map is name of the function.
         */
        std::map<QString, std::unique_ptr<FunctionInfo>> m_functions;

        /**
         * @brief all observers that are part of promela system.
         */
        std::set<QString> m_observers;
    };

    /**
     * @brief Prepare System Information based on model and conversion options.
     *
     * @param model input IVModel
     * @param options conversion options
     * @return system information
     */
    std::unique_ptr<SystemInfo> prepareSystemInfo(const ivm::IVModel *model, const conversion::Options &options) const;

private:
    inline static const QString m_timerManagerProctypeName = "timer_manager_proc";
    inline static const QString m_systemInitedVariableName = "inited";

private:
    auto initializeFunction(model::Sequence &sequence, const QString &functionName) const -> void;
    auto addChannelAndLock(IvToPromelaTranslator::Context &context, const QString &functionName) const -> void;
    auto observerInputSignalName(const IvToPromelaTranslator::ObserverAttachment &attachment) const -> QString;
    auto observerInputSignalName(const ObserverInfo &observerInfo) const -> QString;
    auto attachInputObservers(IvToPromelaTranslator::Context &context, const QString &functionName,
            const QString &interfaceName, const QString &parameterName, const QString &parameterType) const
            -> std::list<std::unique_ptr<promela::model::ProctypeElement>>;
    auto generateInitProctype(Context &context) const -> void;
    auto generateProctype(Context &context, const QString &functionName, bool environment,
            const ProctypeInfo &proctypeInfo) const -> void;
    auto generateProcessMessageBlock(const QString &functionName, const QString &channelName, const QString &inlineName,
            const QString &parameterType, const QString &parameterName, const QString &exitLabel, bool lock,
            std::list<std::unique_ptr<promela::model::ProctypeElement>> preProcessingElements,
            std::list<std::unique_ptr<promela::model::ProctypeElement>> postProcessingElements) const
            -> std::unique_ptr<model::ProctypeElement>;
    auto generateEnvironmentProctype(Context &context, const QString &functionName, const QString &interfaceName,
            const std::pair<QString, QString> &interfaceParameter, const QString &sendInline) const -> void;
    auto generateSendInline(Context &context, const QString &functionName, const QString &interfaceName,
            const QString &parameterName, const QString &parameterType, const QString &sourceFunctionName,
            const QString &sourceInterfaceName) const -> void;
    auto generateUnhandledInputInline(
            Context &context, const QString &functionName, const ProctypeInfo &proctypeInfo) const -> void;
    auto createPromelaObjectsForFunction(Context &context, const ::ivm::IVFunction *ivFunction,
            const QString &functionName, const FunctionInfo &functionInfo) const -> void;
    auto generateProctypeForTimer(Context &context, const ::ivm::IVFunction *ivFunction, const QString &functionName,
            const QString &interfaceName, const ProctypeInfo &proctypeInfo) const -> void;
    auto createPromelaObjectsForAsyncPis(Context &context, const ivm::IVInterface *providedInterface,
            const QString &functionName, const ProctypeInfo &proctypeInfo) const -> void;
    auto createPromelaObjectsForSyncRis(Context &context, const SynchronousCallInfo &info) const -> void;
    auto createPromelaObjectsForEnvironmentSyncRis(Context &context, const EnvSynchronousCallInfo &info) const -> void;
    auto createPromelaObjectsForEnvironment(Context &context, const ivm::IVFunction *ivFunction,
            const QString &functionName, const FunctionInfo &functionInfo) const -> void;
    auto createCheckQueueInline(model::PromelaSystemModel *promelaModel, const QString &functionName,
            const QList<QString> &channelNames) const -> void;
    auto createCheckQueuesExpression(const QList<QString> &channelNames, bool empty) const
            -> std::unique_ptr<::promela::model::Expression>;
    auto createGetSenderInline(model::PromelaSystemModel *promelaModel, const QString &functionName) const -> void;
    auto createSystemState(Context &context) const -> void;
    auto createPromelaObjectsForTimers(Context &context) const -> void;
    auto createTimerInlinesForFunction(Context &context, const QString &functionName, const QString &timerName,
            const promela::model::VariableRef &timerData) const -> void;
    auto createGlobalTimerObjects(
            Context &context, const std::map<QString, promela::model::VariableRef> &timerSignals) const -> void;
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
    auto handleSendInlineArgument(const QString &parameterType, const QString &functionName,
            const QString &interfaceName, const QString parameterName) const -> QString;

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

    auto prepareFunctionInfo(Context &context, const ::ivm::IVFunction *ivFunction, const QString &functionName,
            FunctionInfo &functionInfo) const -> void;
    auto prepareEnvironmentFunctionInfo(Context &context, const ::ivm::IVFunction *ivFunction,
            const QString &functionName, FunctionInfo &functionInfo) const -> void;
    auto prepareProctypeInfo(Context &context, const ivm::IVInterface *providedInterface,
            const QString &functionName) const -> std::unique_ptr<ProctypeInfo>;
    auto prepareEnvProctypeInfo(Context &context, const ivm::IVInterface *requiredInterface,
            const QString &functionName) const -> std::unique_ptr<EnvProctypeInfo>;
    auto prepareSynchronousCallInfo(Context &context, const QString &functionName,
            const ivm::IVInterface *requiredInterface, FunctionInfo &functionInfo) const -> void;
};
}
