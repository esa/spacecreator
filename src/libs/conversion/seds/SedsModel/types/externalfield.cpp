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

#include "types/externalfield.h"

namespace seds::model {

ExternalField::~ExternalField() = default;

const std::optional<ExternalField::Calibrator> &ExternalField::calibrator() const
{
    return m_calibrator;
}

void ExternalField::setCalibrator(ExternalField::Calibrator calibrator)
{
    m_calibrator = std::move(calibrator);
}

const std::optional<DerivedTypeRange> &ExternalField::nominalRange() const
{
    return m_nominalRange;
}

void ExternalField::setNominalRange(DerivedTypeRange nominalRange)
{
    m_nominalRange = std::move(nominalRange);
}

const std::optional<DerivedTypeRange> &ExternalField::safeRange() const
{
    return m_safeRange;
}

void ExternalField::setsafeRange(DerivedTypeRange safeRange)
{
    m_safeRange = std::move(safeRange);
}

} // namespace seds::model
