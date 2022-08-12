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

#include <QVector>
#include <asn1library/asn1/visitor.h>
#include <promela/PromelaModel/promelamodel.h>

namespace promela::translator {
/**
 * @brief Visitor for top-level ASN.1 nodes.
 *
 * This is a part of Asn1ToPromelaTranslator.
 */
class Asn1NodeVisitor : public Asn1Acn::Visitor
{
public:
    /**
     * @brief Constructor.
     *
     * @param promelaModel         target promela model
     * @param enhancedSpinSupport  if true, then generate model for enhanced spin
     */
    Asn1NodeVisitor(model::PromelaModel &promelaModel, bool enhancedSpinSupport);

    /**
     * @brief Getter for init inline names.
     *
     * If the model contain value definitions, then they shall be initialized,
     * by calling inlines. This getter returns inline names to initialize
     * all values.
     *
     * @returns reference to vector with inline names.
     */
    const QVector<QString> getInitInlineNames() const;

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
    model::PromelaModel &m_promelaModel;
    const bool m_enhancedSpinSupport;
    QVector<QString> m_initInlines;

    inline static const QString m_defaultSubtypePostfix = "-DEFAULT-SUBTYPE";
};
}
