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
     * @brief Constructor.
     *
     * @param   promelaModel    Target promela model
     * @param   asn1Model       ASN.1 model
     * @param   ivModel         Interface View model
     */
    Asn1NodeValueGeneratorVisitor(
            model::PromelaModel &promelaModel, const Asn1Acn::Asn1Model *asn1Model, const ivm::IVModel *ivModel);
    /**
     * @brief Constructor.
     *
     * @param   promelaModel    Target promela model
     * @param   typeNames       List of top-level type names to generate value generation inlines
     */
    Asn1NodeValueGeneratorVisitor(model::PromelaModel &promelaModel, QStringList typeNames);

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
    const Asn1Acn::Types::Type *findOverridenType(const QString &subtypeName) const;

private:
    model::PromelaModel &m_promelaModel;
    const Asn1Acn::Asn1Model *m_asn1Model;
    const ivm::IVModel *m_ivModel;
    QStringList m_typeNames;
    bool m_generateSubtypes;
};
}
