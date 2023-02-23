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

#include <QString>
#include <cstdint>
#include <optional>
#include <vector>

namespace promela::translator {

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
}
