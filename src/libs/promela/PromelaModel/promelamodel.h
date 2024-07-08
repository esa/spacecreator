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

#include "cdecl.h"
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
 * @brief   Representation of Promela model.
 */
class PromelaModel : public conversion::Model
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
     * @brief Add C declaration block.
     *
     * @param declaration the cdecl to add to the model
     */
    void addCDecl(CDecl declaration);

    /**
     * @brief Getter for C declaration block.
     *
     * @returns a reference to the cdecl.
     * @throws std::bad_optional_access if there's no cdecl
     */
    const CDecl &getCDecl() const noexcept;

    /**
     * @brief Check if the model has a c declaration block.
     *
     * @returns true if cdecl block is set, otherwise false.
     */
    bool hasCDecl() const noexcept;

    /**
     * @brief Add unnamed mtype value.
     *
     * @param value non empty string value to add
     */
    void addMtype(const QString &value);

    /**
     * @brief Getter for all unnamed mtype values.
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
     * @brief Setter for user defined types.
     *
     * @param utypes list of Utypes to set
     */
    void setUtypes(const QList<Utype> &utypes);

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

    /**
     * @brief Add InlineDef to the model
     *
     * @param inlineDef InlineDef to add to the model
     */
    void addInlineDef(std::unique_ptr<InlineDef> inlineDef);
    /**
     * @brief Getter for all inline definitions
     *
     * @return All inline definitions of the model
     */
    const std::list<std::unique_ptr<InlineDef>> &getInlineDefs() const noexcept;

    /**
     * @brief Add proctype definition to the model
     *
     * @param proctype Proctype to add to the model
     */
    void addProctype(std::unique_ptr<Proctype> proctype);
    /**
     * @brief Getter for all proctypes of promela model
     *
     * @return All proctypes of the model
     */
    const std::list<std::unique_ptr<Proctype>> &getProctypes() const noexcept;

    /**
     * @brief Check if init proctype is present in promela model
     *
     * @return true if init proctype is present, otherwise false
     */
    bool hasInit() const noexcept;
    /**
     * @brief Getter for init proctype of promela model
     *
     * @return init proctype if present
     * @throw std::bad_optional_access if init proctype is not present
     */
    const InitProctype &getInit() const noexcept;
    /**
     * @brief Setter for init proctype
     *
     * @param initProctype init proctype to set in promela model
     */
    void setInit(InitProctype initProctype);

    /**
     * @brief Add epilogue include file
     *
     * File will be included after other definitions
     *
     * @param file filepath to the promela file
     */
    void addEpilogueInclude(const QString &file);

    /**
     * @brief Getter for all epilogue included propmela files
     *
     * @return List of all epilogue included promela files
     */
    const QList<QString> &getEpilogueIncludes() const noexcept;

private:
    std::optional<CDecl> m_cdecl;
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
    QList<QString> m_epilogueIncludes;
};

} // namespace promela::model

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
