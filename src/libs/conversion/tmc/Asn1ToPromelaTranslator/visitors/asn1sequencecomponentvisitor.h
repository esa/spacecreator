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

#include <asn1library/asn1/sequencecomponentvisitor.h>
#include <tmc/PromelaModel/promelamodel.h>
#include <tmc/PromelaModel/utype.h>

namespace conversion::tmc::translator {
/**
 * @brief Visitor for ASN.1 SequenceComponent
 *
 * This is a part of Asn1ToPromelaTranslator
 */
class Asn1SequenceComponentVisitor final : public ::Asn1Acn::SequenceComponentVisitor
{
public:
    /**
     * @brief Contructor.
     *
     * @param model target promela model
     * @param utype target utype
     */
    Asn1SequenceComponentVisitor(::tmc::promela::model::PromelaModel &model, ::tmc::promela::model::Utype &utype);

    /**
     * @brief Visit ::Asn1Acn::AsnSequenceComponent
     *
     * @param component component to visit
     */
    void visit(const ::Asn1Acn::AsnSequenceComponent &component) override;
    /**
     * @brief Visit ::Asn1Acn::AcnSequenceComponent
     *
     * @param component component to visit
     */
    void visit(const ::Asn1Acn::AcnSequenceComponent &component) override;

private:
    ::tmc::promela::model::PromelaModel &m_model;
    ::tmc::promela::model::Utype &m_utype;
};
}
