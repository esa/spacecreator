/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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
#include <asnsequencecomponent.h>
#include <memory>
#include <promela/PromelaModel/proctypeelement.h>
#include <promela/PromelaModel/sequence.h>

using Asn1Acn::AsnSequenceComponent;
using promela::model::ProctypeElement;

namespace promela::translator {
/**
 * @brief Make promela proctype entities
 */
class ProctypeMaker final
{
public:
    static auto makeInlineCall(AsnSequenceComponent *envGeneratorInline, const QString &callArgumentName,
            const QString &generatorInlineName) -> std::unique_ptr<ProctypeElement>;
    static auto makeNormalSequence() -> std::unique_ptr<promela::model::Sequence>;
    static auto makeTrueExpressionProctypeElement() -> std::unique_ptr<ProctypeElement>;
    static auto makeAssignmentProctypeElement(const QString &valueName, int32_t value)
            -> std::unique_ptr<ProctypeElement>;
};

} // namespace promela::translator
