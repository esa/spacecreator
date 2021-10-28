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

#include "namedmtype.h"
#include "typealias.h"
#include "utype.h"
#include "valuedefinition.h"

#include <QList>
#include <QSet>
#include <QString>
#include <conversion/common/model.h>
#include <conversion/common/modelproperties.h>
#include <map>

namespace tmc::promela::model {
/**
 * @brief Representation of Promela model.
 */
class PromelaModel final : public conversion::Model
{
public:
    /// @brief Default constructor.
    PromelaModel() = default;

    /// @brief Deleted copy constructor.
    PromelaModel(const PromelaModel &) = delete;
    /// @brief Default move constructor.
    PromelaModel(PromelaModel &&) = default;

    /// @brief Deleted copy assignment operator.
    PromelaModel &operator=(const PromelaModel &) = delete;
    /// @brief Default move assignment operator.
    PromelaModel &operator=(PromelaModel &&) = default;

    /**
     * @brief   Get this model type
     *
     * @return  Model type
     */
    auto modelType() const -> conversion::ModelType override;

    /**
     * @brief Add unnamed mtype value.
     *
     * @param value non empty string value to add
     */
    void addMtype(const QString &value);

    /**
     * @brief Getter fo all unnamed mtype values.
     *
     * @return Unnamed mtype values
     */
    const QSet<QString> &getMtypeValues() const noexcept;

    /**
     * @brief Add named mtype.
     *
     * @param mtype NamedMtype to add to the model
     */
    void addNamedMtype(const NamedMtype &mtype);
    /**
     * @brief Getter for all named mtypes.
     *
     * @return All named mtypes.
     */
    const std::map<QString, NamedMtype> &getNamedMtypeValues() const noexcept;

    /**
     * @brief Add Utype - user defined type to the model.
     *
     * @param utype Utype to add to the model
     */
    void addUtype(const Utype &utype);
    /**
     * @brief Getter for all user defined types.
     *
     * @return All Utypes of the model
     */
    const QList<Utype> &getUtypes() const noexcept;

    /**
     * @brief Add TypeAlias - alias for type to the model.
     *
     * @param alias TypeAlias to add to the model
     */
    void addTypeAlias(const TypeAlias &alias);
    /**
     * @brief Getter for all type aliases in model
     *
     * @return All type aliases of the model
     */
    const QList<TypeAlias> &getTypeAliases() const noexcept;

    /**
     * @brief Add ValueDefinition value definition to the model
     *
     * @param value ValueDefinition to add to the model
     */
    void addValueDefinition(const ValueDefinition &value);

    /**
     * @brief Getter for all value definitions of promela model
     *
     * @return All value definitions of the model
     */
    const QList<ValueDefinition> &getValueDefinitions() const noexcept;

private:
    QSet<QString> m_mtypeValues;
    std::map<QString, NamedMtype> m_namedMtypeValues;
    QList<Utype> m_userTypes;
    std::map<QString, Declaration> m_declarations;
    QList<TypeAlias> m_typeAliases;
    QList<ValueDefinition> m_valueDefinitions;
};
}
namespace conversion {
/**
 * @brief   Specialization for SEDS model properties
 */
template<>
struct ModelProperties<::tmc::promela::model::PromelaModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::Seds;
};
}
