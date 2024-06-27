/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2023 N7 Space Sp. z o.o.
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

#include <QList>
#include <QMap>
#include <QString>
#include <cstddef>
#include <list>
#include <map>
#include <set>

namespace promela::translator {
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
    /**
     * @brief Name of the channel which observer sends message
     */
    QString m_nextQueue;
    /**
     * @brief Name of the function after 'to' or empty
     *
     * Name of the receiving function.
     */
    QString m_toFunction;
};

/**
 * @brief Information about proctype in promela
 */
struct ProctypeInfo {
    /**
     * @brief Name of the proctype in promela
     */
    QString m_proctypeName;
    /**
     * @brief Name of the interface which proctype originates from
     */
    QString m_interfaceName;
    /**
     * @brief Name of the channel in promela used to receive signals
     */
    QString m_queueName;
    /**
     * @brief Size of channel in promela
     */
    size_t m_queueSize;
    /**
     * @brief Priority of proctype
     */
    size_t m_priority;
    /**
     * @brief Parameter type name (or empty for parameterless interfaces)
     */
    QString m_parameterTypeName;
    /**
     * @brief Parameter name (or empty for parameterless interfaces)
     */
    QString m_parameterName;
    /**
     * @brief true if proctype originates from timer
     */
    bool m_isTimer;

    /**
     * @brief The map of possible senders
     *
     * The key is name of IV function
     * The value is name of interface
     * In case of timer, this is empty container
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
     * @brief Name of the proctype in promela
     */
    QString m_proctypeName;
    /**
     * @brief Name of the interface which proctype originates from
     */
    QString m_interfaceName;
    /**
     * @brief Name of the parameter type or empty string
     */
    QString m_parameterType;
    /**
     * @brief Name of the parameter or empty string
     */
    QString m_parameterName;
    /**
     * @brief Name of the promela inline, to call
     */
    QString m_sendInlineName;
    /**
     * @brief Priority of proctype
     */
    size_t m_priority;
};

/**
 * @brief Information about required call
 *
 * This is used to call the target functions interface
 */
struct RequiredCallInfo {
    /**
     * @brief Name of the inline
     */
    QString m_name;

    /**
     * @brief Name name of the interface
     */
    QString m_interfaceName;

    /**
     * @brief true if function requires lock
     */
    bool m_isProtected;

    /**
     * @brief Container to hold information about target of required interface
     */
    struct TargetInfo {
        /**
         * @brief Name of the inline to call (provided) or empty in case of environment
         */
        QString m_providedInlineName;
        /**
         * @brief Name of the queue to send message (if call is sporadic)
         */
        QString m_providedQueueName;
        /**
         * @brief Name of the target function
         */
        QString m_targetFunctionName;

        /**
         * @brief Flag to indicate that target is environment
         */
        bool m_isEnvironment;
    };

    /**
     * @brief List of targets
     */
    std::map<QString, TargetInfo> m_targets;

    /**
     * @brief Information about parameter
     */
    struct ParameterInfo {
        /**
         * @brief Name of the parameter
         */
        QString m_parameterName;
        /**
         * @brief Type of the parameter
         */
        QString m_parameterType;
        /**
         * @brief true if the parameter is output parameter

         The value for output parameters shall be generated by env in case when the
         target is an environment function.
         */
        bool m_isOutput;
    };

    /**
     * @brief List of parameters
     */
    QList<ParameterInfo> m_parameters;
};

/**
 * @brief Representation of information about IV function in promela system
 *
 * Every function consists of set proctypes
 */
struct FunctionInfo {
    /**
     * @brief The flag to mark IV function as environment
     */
    bool m_isEnvironment;
    /**
     * @brief The name of the function type or an empty string
     */
    QString m_functionType;
    /**
     * @brief All model proctypes of IV function
     *
     * When the function is a model function, m_proctypes shall be non empty
     * When the function is a part of environment, this container shall be empty
     */
    std::map<QString, std::unique_ptr<ProctypeInfo>> m_proctypes;
    /**
     * @brief All environment proctypes, which are used to generate value
     *
     * This container shall be empty for model functions
     */
    std::map<QString, std::unique_ptr<EnvProctypeInfo>> m_environmentSourceProctypes;
    /**
     * @brief All environment proctypes, which are used to receive signals from model functions
     *
     * This container shall be empty for model functions
     */
    std::map<QString, std::unique_ptr<ProctypeInfo>> m_environmentSinkProctypes;

    /**
     * @brief All required synchronous calls
     *
     * The key is name of the inline for RI
     * The value contains all information required to create body of inline
     */
    std::map<QString, std::unique_ptr<RequiredCallInfo>> m_synchronousCalls;
    /**
     * @brief All required asynchronous calls
     * The key is name of the inline for RI
     * The vaule contains all information required to create body of inline
     */
    std::map<QString, std::unique_ptr<RequiredCallInfo>> m_sporadicCalls;
};

/**
 * @brief Representation of structure of promela system
 */
struct SystemInfo {
    /**
     * @brief All functions from IV that are part of promela system
     *
     * This map includes environment functions. The key in the map is name of the function
     */
    std::map<QString, std::unique_ptr<FunctionInfo>> m_functions;

    /**
     * @brief All observers that are a part of promela system
     */
    std::set<QString> m_observers;

    std::set<QString> m_messageTypes;
};

}
