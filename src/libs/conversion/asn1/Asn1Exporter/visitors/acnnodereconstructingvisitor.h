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

#include <QString>
#include <QTextStream>

#include <data/visitor.h>

namespace Asn1Acn {

class AcnNodeReconstructingVisitor : public Data::Visitor
{
public:
    AcnNodeReconstructingVisitor(QTextStream &outStream);
    ~AcnNodeReconstructingVisitor() override = default;

    void visit(const Data::Definitions &defs) override;
    void visit(const Data::File &file) override;
    void visit(const Data::TypeAssignment &type) override;
    void visit(const Data::ValueAssignment &value) override;
    void visit(const Data::Project &project) override;
    void visit(const Data::Root &root) override;

private:
    template<typename T>
    void reconstructCollection(const T &collection) const;

    QTextStream &m_outStream;
};

} // namespace Asn1Acn
