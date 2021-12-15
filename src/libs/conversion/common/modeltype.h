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

class QString;

#include <QString>
#include <QStringList>
#include <set>

namespace conversion {

/**
 * @brief   All model types supported in conversion
 */
enum class ModelType
{
    Unspecified,
    Asn1,
    InterfaceView,
    Promela,
    Sdl,
    Seds
};

/**
 * @brief   Converts given model type to string
 *
 * @param   modelType   Model type to convert
 *
 * @param   String with model type name
 */
auto modelTypeToString(ModelType modelType) -> const QString &;
/**
 * @brief   Converts given set of model types to string
 *
 * @param   sourceModelsTypes       Set of model types
 *
 * @return  String with model types names separated with comma
 */
auto modelTypesToString(const std::set<ModelType> &modelsTypes) -> QString;
/**
 * @brief   Returns given model type extensions
 *
 * @param   modelType   Model type
 *
 * @param   String with model type extension
 */
auto modelTypeExtensions(ModelType modelType) -> const QStringList &;
/**
 * @brief   Converts given model type to string
 *
 * @param   s   String with model type name
 *
 * @param   Model type from string
 */
auto stringToModelType(const QString &s) -> ModelType;

/**
 * @brief   Trait for properties of model types
 *
 * @tparam  ModelT      Model type
 */
template<ModelType ModelT>
struct ModelTypeProperties {
    /// @brief  Model type name
    static inline const QString name = "Unspecified";
    /// @brief  Model type extensions
    static inline const QStringList extensions = { ".none" };
};

/**
 * @brief   Specialization for ASN.1 model type
 */
template<>
struct ModelTypeProperties<ModelType::Asn1> {
    /// @brief  Model name
    static inline const QString name = "ASN.1";
    /// @brief  Model extensions
    static inline const QStringList extensions = { ".asn", ".asn1", ".acn" };
};

/**
 * @brief   Specialization for InterfaceView model type
 */
template<>
struct ModelTypeProperties<ModelType::InterfaceView> {
    /// @brief  Model name
    static inline const QString name = "InterfaceView";
    /// @brief  Model extensions
    static inline const QStringList extensions = { ".xml" };
};

/**
 * @brief   Specialization for Promela model type
 */
template<>
struct ModelTypeProperties<ModelType::Promela> {
    /// @brief  Model name
    static inline const QString name = "Promela";
    /// @brief  Model extension
    static inline const QStringList extensions = { ".pml" };
};

/**
 * @brief   Specialization for SDL model type
 */
template<>
struct ModelTypeProperties<ModelType::Sdl> {
    /// @brief  Model name
    static inline const QString name = "SDL";
    /// @brief  Model extension
    static inline const QStringList extensions = { ".pr" };
};

/**
 * @brief   Specialization for SEDS model type
 */
template<>
struct ModelTypeProperties<ModelType::Seds> {
    /// @brief  Model name
    static inline const QString name = "SEDS";
    /// @brief  Model extension
    static inline const QStringList extensions = { ".xml" };
};

} // namespace conversion
