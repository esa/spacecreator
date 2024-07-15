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

#include <QStringList>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/visitor.h>
#include <ivcore/ivmodel.h>
#include <promela/PromelaModel/promelamodel.h>

namespace promela::translator {
/**
 * @brief Main visitor responsible for generation of promela inlines
 * for environment values generation.
 */
class Asn1NodeValueGeneratorVisitor : public Asn1Acn::Visitor
{
public:
    /**
     * @brief Create visitor to generate inlines for environment subtypes.
     * The visitor requires access to iv model to obtain original type.
     *
     * @param   promelaModel    Target promela model
     * @param   asn1Model       ASN.1 model
     * @param   ivModel         Interface View model
     * @param   delta           Space between generated floats
     */
    static Asn1NodeValueGeneratorVisitor generatorForEnvironmentSubtypes(model::PromelaModel &promelaModel,
            const Asn1Acn::Asn1Model *asn1Model, const ivm::IVModel *ivModel,
            const std::optional<float> &delta = std::nullopt);

    /**
     * @brief Create visitor do generate inlines for model types.
     *
     * @param   promelaModel    Target promela model
     * @param   typeNames       List of top-level type names to generate value generation inlines
     * @param   delta           Space between generated floats
     */
    static Asn1NodeValueGeneratorVisitor generatorForModelTypes(model::PromelaModel &promelaModel,
            const QStringList &typeNames, const std::optional<float> &delta = std::nullopt);

public:
    /// @brief Visit Asn1Acn::Definitions
    void visit(const Asn1Acn::Definitions &defs) override;
    /// @brief Visit Asn1Acn::File
    void visit(const Asn1Acn::File &file) override;
    /// @brief Visit Asn1Acn::TypeAssignment
    void visit(const Asn1Acn::TypeAssignment &type) override;
    /// @brief Visit Asn1Acn::ValueAssignment
    void visit(const Asn1Acn::ValueAssignment &value) override;
    /// @brief Visit Asn1Acn::Project
    void visit(const Asn1Acn::Project &project) override;
    /// @brief Visit Asn1Acn::Root
    void visit(const Asn1Acn::Root &root) override;

private:
    Asn1NodeValueGeneratorVisitor(model::PromelaModel &promelaModel, const Asn1Acn::Asn1Model *asn1Model,
            const ivm::IVModel *ivModel, const std::optional<float> &delta);
    Asn1NodeValueGeneratorVisitor(
            model::PromelaModel &promelaModel, QStringList typeNames, const std::optional<float> &delta);

    const Asn1Acn::Types::Type *findOriginalIvParameterType(const QString &subtypeName) const;
    QMap<QString, QString> prepareEnvironmentSubtypeToIvTypeMap() const;

private:
    model::PromelaModel &m_promelaModel;
    const Asn1Acn::Asn1Model *m_asn1Model;
    const ivm::IVModel *m_ivModel;
    QStringList m_typeNames;
    bool m_isEnvironmentSubtype;
    std::optional<float> m_delta;
    QMap<QString, QString> m_environmentSubtypeToIvTypeMapping;
};
}
