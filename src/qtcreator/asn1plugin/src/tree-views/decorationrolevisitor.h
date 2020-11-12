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

#include <QIcon>

#include <asn1/visitorwithvalue.h>

#include <utils/theme/theme.h>

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {

class DecorationRoleVisitor : public Asn1Acn::VisitorWithValue<QIcon>
{
public:
    ~DecorationRoleVisitor() override;

private:
    QIcon valueFor(const Asn1Acn::Root &root) const override;
    QIcon valueFor(const Asn1Acn::Definitions &defs) const override;
    QIcon valueFor(const Asn1Acn::File &file) const override;
    QIcon valueFor(const Asn1Acn::TypeAssignment &type) const override;
    QIcon valueFor(const Asn1Acn::ValueAssignment &value) const override;
    QIcon valueFor(const Asn1Acn::Project &project) const override;
};

QIcon createIcon(const QString &iconFile, Utils::Theme::Color color);
QIcon typeIcon(const QString &iconFile);
QIcon valueIcon(const QString &iconFile);

} // namespace TreeViews
} // namespace Internal
} // namespace Asn1Acn
