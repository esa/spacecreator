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

#include "ivtopromelatranslatorcontext.h"

#include <promela/PromelaOptions/options.h>

using conversion::promela::PromelaOptions;
using ivm::IVModel;

namespace promela::translator {
IvToPromelaTranslatorContext::IvToPromelaTranslatorContext(model::PromelaSystemModel *promelaModel,
        const IVModel *ivModel, const Asn1Acn::Asn1Model *asn1Model, const conversion::Options &options,
        const std::vector<const Asn1Acn::Definitions *> &asn1SubtypesDefinitons,
        const std::vector<QString> &modelFunctions, const std::vector<QString> &observerNames)
    : m_promelaModel(promelaModel)
    , m_ivModel(ivModel)
    , m_asn1Model(asn1Model)
    , m_options(options)
    , m_asn1SubtypesDefinitons(asn1SubtypesDefinitons)
    , m_modelFunctions(modelFunctions)
    , m_observerNames(observerNames)
    , m_baseProctypePriority(0)
    , m_supportMulticast(options.isSet(PromelaOptions::supportMulticast))
{
}

void IvToPromelaTranslatorContext::addObserverAttachment(const ObserverAttachment &attachment)
{
    if (attachment.kind() == ObserverAttachment::Kind::Kind_Unhandled_Input) {
        const QString toFunction = attachment.toFunction().has_value() ? attachment.toFunction().value() : QString();

        m_unhandledInputObserverAttachments[toFunction][attachment.interface()].push_back(attachment);
    } else {
        if (attachment.fromFunction().has_value()) {
            m_fromObserverAttachments[*attachment.fromFunction()][attachment.interface()].push_back(attachment);
        }
        if (attachment.toFunction().has_value()) {
            m_toObserverAttachments[*attachment.toFunction()][attachment.interface()].push_back(attachment);
        }
        if (attachment.kind() == ObserverAttachment::Kind::Kind_Continuous_Signal) {
            m_observersWithContinuousSignals.push_back(attachment.observer());
        }
    }
}

auto IvToPromelaTranslatorContext::getObserverAttachments(const QString &function, const QString &interface,
        const ObserverAttachment::Kind kind) -> const ObserverAttachments
{
    ObserverAttachments result;
    if (m_toObserverAttachments.find(function) == m_toObserverAttachments.end()) {
        return result;
    }
    const auto &attachments = m_toObserverAttachments.at(function);
    if (attachments.find(interface) == attachments.end()) {
        return result;
    }
    for (const auto &attachment : attachments.at(interface)) {
        if (attachment.kind() == kind) {
            result.push_back(attachment);
        }
    }
    std::sort(result.begin(), result.end(),
            [](const auto &a, const auto &b) -> bool { return a.priority() > b.priority(); });
    return result;
}

auto IvToPromelaTranslatorContext::hasObserverAttachments(
        const QString &function, const QString &interface, const ObserverAttachment::Kind kind) -> bool
{
    return getObserverAttachments(function, interface, kind).size() > 0;
}

auto IvToPromelaTranslatorContext::getObserverAttachments(const ObserverAttachment::Kind kind)
        -> const ObserverAttachments
{
    ObserverAttachments result;
    if (kind == ObserverAttachment::Kind::Kind_Output) {
        for (auto functionIter = m_fromObserverAttachments.begin(); functionIter != m_fromObserverAttachments.end();
                ++functionIter) {
            for (auto interfaceIter = functionIter->second.begin(); interfaceIter != functionIter->second.end();
                    ++interfaceIter) {
                std::copy_if(interfaceIter->second.begin(), interfaceIter->second.end(), std::back_inserter(result),
                        [kind](const ObserverAttachment &attachment) { return attachment.kind() == kind; });
            }
        }
    } else if (kind == ObserverAttachment::Kind::Kind_Input) {
        for (auto functionIter = m_toObserverAttachments.begin(); functionIter != m_toObserverAttachments.end();
                ++functionIter) {
            for (auto interfaceIter = functionIter->second.begin(); interfaceIter != functionIter->second.end();
                    ++interfaceIter) {
                std::copy_if(interfaceIter->second.begin(), interfaceIter->second.end(), std::back_inserter(result),
                        [kind](const ObserverAttachment &attachment) { return attachment.kind() == kind; });
            }
        }
    }
    return result;
}

auto IvToPromelaTranslatorContext::getUnhandledInputObserversForFunction(
        const QString &function, const QString &interface) const -> ObserverAttachments
{
    ObserverAttachments result;

    if (m_unhandledInputObserverAttachments.count(function) > 0) {
        if (m_unhandledInputObserverAttachments.at(function).count(interface) > 0) {
            std::copy_if(m_unhandledInputObserverAttachments.at(function).at(interface).begin(),
                    m_unhandledInputObserverAttachments.at(function).at(interface).end(), std::back_inserter(result),
                    [](const ObserverAttachment &attachment) {
                        return attachment.kind() == ObserverAttachment::Kind::Kind_Unhandled_Input;
                    });
        }
        if (m_unhandledInputObserverAttachments.at(function).count("") > 0) {
            std::copy_if(m_unhandledInputObserverAttachments.at(function).at("").begin(),
                    m_unhandledInputObserverAttachments.at(function).at("").end(), std::back_inserter(result),
                    [](const ObserverAttachment &attachment) {
                        return attachment.kind() == ObserverAttachment::Kind::Kind_Unhandled_Input;
                    });
        }
    }
    if (m_unhandledInputObserverAttachments.count("") > 0) {
        if (m_unhandledInputObserverAttachments.at("").count("") > 0) {
            std::copy_if(m_unhandledInputObserverAttachments.at("").at("").begin(),
                    m_unhandledInputObserverAttachments.at("").at("").end(), std::back_inserter(result),
                    [](const ObserverAttachment &attachment) {
                        return attachment.kind() == ObserverAttachment::Kind::Kind_Unhandled_Input;
                    });
        }
    }

    std::sort(result.begin(), result.end(),
            [](const auto &a, const auto &b) -> bool { return a.priority() > b.priority(); });
    return result;
}

auto IvToPromelaTranslatorContext::model() const -> model::PromelaSystemModel *
{
    return m_promelaModel;
}

const IVModel *IvToPromelaTranslatorContext::ivModel() const
{
    return m_ivModel;
}

const Asn1Acn::Asn1Model *IvToPromelaTranslatorContext::asn1Model() const
{
    return m_asn1Model;
}

const conversion::Options &IvToPromelaTranslatorContext::options() const
{
    return m_options;
}

const std::vector<const Asn1Acn::Definitions *> &IvToPromelaTranslatorContext::subtypesDefinitions() const
{
    return m_asn1SubtypesDefinitons;
}

const std::vector<QString> &IvToPromelaTranslatorContext::modelFunctions() const
{
    return m_modelFunctions;
}

const std::vector<QString> &IvToPromelaTranslatorContext::observerNames() const
{
    return m_observerNames;
}

void IvToPromelaTranslatorContext::setBaseProctypePriority(uint32_t priority)
{
    m_baseProctypePriority = priority;
}

uint32_t IvToPromelaTranslatorContext::getBaseProctypePriority() const
{
    return m_baseProctypePriority;
}

bool IvToPromelaTranslatorContext::isMulticastSupported() const
{
    return m_supportMulticast;
}

const std::vector<QString> &IvToPromelaTranslatorContext::getObserversWithContinuousSignals() const
{
    return m_observersWithContinuousSignals;
}
}
