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

#include "initproctype.h"
#include "inlinedef.h"
#include "namedmtype.h"
#include "proctype.h"
#include "proctypeelement.h"
#include "typealias.h"
#include "utype.h"
#include "valuedefinition.h"

#include <QList>
#include <QSet>
#include <QString>
#include <conversion/common/model.h>
#include <conversion/common/modelproperties.h>
#include <list>
#include <map>
#include <memory>

namespace promela::model {
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
     * @brief Add include file
     *
     * @param file filepath to the promela file
     */
    void addInclude(const QString &file);

    /**
     * @brief Getter for all included propmela files
     *
     * @return List of included promela files
     */
    const QList<QString> &getIncludes() const noexcept;

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

    /**
     * @brief Add Declaration to the model
     *
     * @param declaration Declaration to add to the model
     */
    void addDeclaration(const Declaration &declaration);

    /**
     * @brief Getter for all declarations of promela model
     *
     * @return All declarations of the model
     */
    const QList<Declaration> &getDeclarations() const noexcept;

    void addInlineDef(std::unique_ptr<InlineDef> inlineDef);
    const std::list<std::unique_ptr<InlineDef>> &getInlineDefs() const noexcept;

    void addProctype(std::unique_ptr<Proctype> proctype);
    const std::list<std::unique_ptr<Proctype>> &getProctypes() const noexcept;

    bool hasInit() const noexcept;
    const InitProctype &getInit() const noexcept;
    void setInit(InitProctype initProctype);

private:
    QList<QString> m_includes;
    QSet<QString> m_mtypeValues;
    std::map<QString, NamedMtype> m_namedMtypeValues;
    QList<Utype> m_userTypes;
    QList<TypeAlias> m_typeAliases;
    QList<ValueDefinition> m_valueDefinitions;
    QList<Declaration> m_declarations;
    std::list<std::unique_ptr<InlineDef>> m_inlineDefs;
    std::list<std::unique_ptr<Proctype>> m_proctypes;
    std::optional<InitProctype> m_initProctype;
};
}

namespace conversion {

/**
 * @brief   Specialization for Promela model
 */
template<>
struct ModelProperties<::promela::model::PromelaModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::Promela;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QStringList extensions = ModelTypeProperties<type>::extensions;
};

} // namespace conversion
