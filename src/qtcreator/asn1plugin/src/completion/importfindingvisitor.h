/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
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

#include <QString>

#include <asn1/visitorwithvalue.h>

namespace Asn1Acn {

class Node;

namespace Internal {
namespace Completion {

class ImportFindingVisitor : public Asn1Acn::VisitorWithValue<Asn1Acn::Node *>
{
public:
    ImportFindingVisitor(const QString &module, const QString &import);

private:
    Asn1Acn::Node *valueFor(const Asn1Acn::Root &root) const override;
    Asn1Acn::Node *valueFor(const Asn1Acn::Definitions &defs) const override;
    Asn1Acn::Node *valueFor(const Asn1Acn::File &file) const override;
    Asn1Acn::Node *valueFor(const Asn1Acn::TypeAssignment &type) const override;
    Asn1Acn::Node *valueFor(const Asn1Acn::ValueAssignment &value) const override;
    Asn1Acn::Node *valueFor(const Asn1Acn::Project &project) const override;

    bool isValue() const;
    template<typename Collection>
    Asn1Acn::Node *findImportByName(const Collection &col) const;

    QString m_module;
    QString m_import;
};

} // namespace Completion
} /* namespace Internal */
} /* namespace Asn1Acn */
