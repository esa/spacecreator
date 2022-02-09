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

#include "modelproperties.h"

#include <QString>
#include <numeric>

namespace conversion {

const QString &modelTypeToString(ModelType modelType)
{
    switch (modelType) {
    case ModelType::Unspecified:
        return ModelTypeProperties<ModelType::Unspecified>::name;
    case ModelType::Asn1:
        return ModelTypeProperties<ModelType::Asn1>::name;
    case ModelType::InterfaceView:
        return ModelTypeProperties<ModelType::InterfaceView>::name;
    case ModelType::Promela:
        return ModelTypeProperties<ModelType::Promela>::name;
    case ModelType::Sdl:
        return ModelTypeProperties<ModelType::Sdl>::name;
    case ModelType::Seds:
        return ModelTypeProperties<ModelType::Seds>::name;
    default:
        static const auto unhandled = QStringLiteral("unhandled model type name");
        return unhandled;
    }
}

QString modelTypesToString(const std::set<ModelType> &modelsTypes)
{
    return std::accumulate(std::next(modelsTypes.begin()), modelsTypes.end(), modelTypeToString(*modelsTypes.begin()),
            [](const QString &acc, ModelType modelType) { return acc + ", " + modelTypeToString(modelType); });
}

const QStringList &modelTypeExtensions(ModelType modelType)
{
    switch (modelType) {
    case ModelType::Unspecified:
        return ModelTypeProperties<ModelType::Unspecified>::extensions;
    case ModelType::Asn1:
        return ModelTypeProperties<ModelType::Asn1>::extensions;
    case ModelType::InterfaceView:
        return ModelTypeProperties<ModelType::InterfaceView>::extensions;
    case ModelType::Promela:
        return ModelTypeProperties<ModelType::Promela>::extensions;
    case ModelType::Sdl:
        return ModelTypeProperties<ModelType::Sdl>::extensions;
    case ModelType::Seds:
        return ModelTypeProperties<ModelType::Seds>::extensions;
    default:
        static const QStringList unhandled = { QStringLiteral("unhandled model type extension") };
        return unhandled;
    }
}

ModelType stringToModelType(const QString &s)
{
    if (!s.compare(ModelTypeProperties<ModelType::Asn1>::name, Qt::CaseInsensitive)) {
        return conversion::ModelType::Asn1;
    }
    if (!s.compare(ModelTypeProperties<ModelType::Seds>::name, Qt::CaseInsensitive)) {
        return conversion::ModelType::Seds;
    }
    if (!s.compare(ModelTypeProperties<ModelType::InterfaceView>::name, Qt::CaseInsensitive)) {
        return conversion::ModelType::InterfaceView;
    }
    if (!s.compare(ModelTypeProperties<ModelType::Sdl>::name, Qt::CaseInsensitive)) {
        return conversion::ModelType::Sdl;
    }
    if (!s.compare(ModelTypeProperties<ModelType::Promela>::name, Qt::CaseInsensitive)) {
        return conversion::ModelType::Promela;
    }
    return conversion::ModelType::Unspecified;
}

} // namespace conversion
