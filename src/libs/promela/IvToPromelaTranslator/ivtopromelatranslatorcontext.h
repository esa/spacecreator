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

#include "observerattachment.h"

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/definitions.h>
#include <conversion/common/options.h>
#include <ivcore/ivinterface.h>
#include <promela/PromelaModel/promelasystemmodel.h>

namespace promela::translator {
/**
 * @brief Translation context
 */
class IvToPromelaTranslatorContext
{
public:
    /**
     * @brief Constructor
     *
     * @param promelaModel The model that is being created
     */
    IvToPromelaTranslatorContext(model::PromelaSystemModel *promelaModel, const ivm::IVModel *ivModel,
            const Asn1Acn::Asn1Model *asn1Model, const conversion::Options &options,
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
    auto getObserverAttachments(const QString &function, const QString &interface, const ObserverAttachment::Kind kind)
            -> const ObserverAttachments;

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
    auto hasObserverAttachments(const QString &function, const QString &interface, const ObserverAttachment::Kind kind)
            -> bool;

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

    auto asn1Model() const -> const Asn1Acn::Asn1Model*;

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
     * @brief Getter for support multicast flag
     *
     * @return true if multicast is enabled
     */
    auto isMulticastSupported() const -> bool;

    /**
     * @brief Getter for vector of observers with continuous signals
     *
     * @return vector of observers with continuous signals
     */
    auto getObserversWithContinuousSignals() const -> const std::vector<QString> &;

private:
    model::PromelaSystemModel *m_promelaModel;
    const ivm::IVModel *m_ivModel;
    const Asn1Acn::Asn1Model *m_asn1Model;
    const conversion::Options &m_options;
    const std::vector<const Asn1Acn::Definitions *> &m_asn1SubtypesDefinitons;
    const std::vector<QString> &m_modelFunctions;
    const std::vector<QString> &m_observerNames;
    uint32_t m_baseProctypePriority;
    const bool m_supportMulticast;
    std::map<QString, std::map<QString, ObserverAttachments>> m_fromObserverAttachments;
    std::map<QString, std::map<QString, ObserverAttachments>> m_toObserverAttachments;
    std::vector<QString> m_observersWithContinuousSignals;
    std::map<QString, std::map<QString, ObserverAttachments>> m_unhandledInputObserverAttachments;
};
}
