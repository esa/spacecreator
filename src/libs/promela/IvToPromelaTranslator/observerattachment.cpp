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

#include "observerattachment.h"

#include <conversion/common/exceptions.h>
#include <conversion/common/translation/exceptions.h>

using conversion::translator::TranslationException;

namespace promela::translator {
ObserverAttachment::Kind ObserverAttachment::stringToKind(const QString &kind)
{
    const auto kindIn = QString("ObservedSignalKind.INPUT");
    const auto kindOut = QString("ObservedSignalKind.OUTPUT");
    const auto kindContinuousSignal = QString("ObservedSignalKind.CONTINUOUS_SIGNAL");
    const auto kindUnhandledInput = QString("ObservedSignalKind.UNHANDLED_INPUT");
    if (kind == kindIn) {
        return ObserverAttachment::Kind::Kind_Input;
    } else if (kind == kindOut) {
        return ObserverAttachment::Kind::Kind_Output;
    } else if (kind == kindContinuousSignal) {
        return ObserverAttachment::Kind::Kind_Continuous_Signal;
    } else if (kind == kindUnhandledInput) {
        return ObserverAttachment::Kind::Kind_Unhandled_Input;
    } else {
        const auto message = QString("Observer kind %1 is unknown").arg(kind);
        throw TranslationException(message);
    }
}

ObserverAttachment::ObserverAttachment(const QString &specification)
{
    const auto separator = QString(":");
    const auto recipientPrefix = QString(">");
    const auto senderPrefix = QString("<");
    const auto priorityPrefix = QString("p");

    const auto elements = specification.split(separator, Qt::KeepEmptyParts);
    if (elements.size() < 4) {
        const auto message =
                QString("Observer attachment specification <%1> contains too few elements").arg(specification);
        throw TranslationException(message);
    }
    m_observerName = elements[0];
    m_kind = stringToKind(elements[1]);
    m_observerInterfaceName = elements[2];
    m_interfaceName = elements[3];
    m_priority = 1;

    for (auto i = 4; i < elements.size(); i++) {
        if (elements[i].startsWith(recipientPrefix)) {
            m_toFunctionName = elements[i].right(elements[i].length() - 1);
        } else if (elements[i].startsWith(senderPrefix)) {
            m_fromFunctionName = elements[i].right(elements[i].length() - 1);
        } else if (elements[i].startsWith(priorityPrefix)) {
            bool ok = false;
            const auto priorityString = elements[i].right(elements[i].length() - 1);
            m_priority = priorityString.toUInt(&ok);
            if (!ok) {
                const auto message = QString("Priority %1 could not be parsed as an integer").arg(priorityString);
                throw TranslationException(message);
            }
        }
    }

    if (m_interfaceName.isEmpty() && m_kind != Kind::Kind_Continuous_Signal && m_kind != Kind::Kind_Unhandled_Input) {
        const auto message = QString("Observed interface name is empty in observer %1 ").arg(m_observerName);
        throw TranslationException(message);
    }
}

std::optional<QString> ObserverAttachment::toFunction() const
{
    return m_toFunctionName;
}

std::optional<QString> ObserverAttachment::fromFunction() const
{
    return m_fromFunctionName;
}

const QString &ObserverAttachment::observerInterface() const
{
    return m_observerInterfaceName;
}

const QString &ObserverAttachment::interface() const
{
    return m_interfaceName;
}

const QString &ObserverAttachment::observer() const
{
    return m_observerName;
}

ObserverAttachment::Kind ObserverAttachment::kind() const
{
    return m_kind;
}

ObserverAttachment::Priority ObserverAttachment::priority() const
{
    return m_priority;
}
}
