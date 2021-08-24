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

#include "modeltype.h"

#include <QString>
#include <numeric>

namespace conversion {

QString modelTypeToString(ModelType modelType)
{
    switch (modelType) {
    case ModelType::Asn1:
        return "ASN.1";
    case ModelType::Aadl:
        return "AADL";
    case ModelType::Sdl:
        return "SDL";
    case ModelType::Seds:
        return "SEDS";
    default:
        return "unhandled model type";
    }
}

QString modelTypesToString(const std::set<ModelType> &modelsTypes)
{
    return std::accumulate(std::next(modelsTypes.begin()), modelsTypes.end(), modelTypeToString(*modelsTypes.begin()),
            [](QString acc, ModelType modelType) { return acc + ", " + modelTypeToString(modelType); });
}

} // namespace conversion
