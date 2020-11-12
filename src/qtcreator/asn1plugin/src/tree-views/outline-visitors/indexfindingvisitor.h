/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#pragma once

#include <asn1/node.h>
#include <asn1/visitorwithvalue.h>

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {
namespace OutlineVisitors {

class IndexFindingVisitor : public Asn1Acn::VisitorWithValue<int>
{
public:
    IndexFindingVisitor(const Asn1Acn::Node *child);
    ~IndexFindingVisitor() override;

    const Asn1Acn::Node *child() const { return m_child; }

private:
    int valueFor(const Asn1Acn::Definitions &defs) const override;
    int valueFor(const Asn1Acn::File &file) const override;
    int valueFor(const Asn1Acn::TypeAssignment &type) const override;
    int valueFor(const Asn1Acn::ValueAssignment &value) const override;
    int valueFor(const Asn1Acn::Project &project) const override;
    int valueFor(const Asn1Acn::Root &root) const override;

    const Asn1Acn::Node *m_child;

    template<typename Collection>
    int findIndexIn(const Collection &items) const;
};

} // namespace OutlineVisitors
} // namespace TreeViews
} // namespace Internal
} // namespace Asn1Acn
