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
#include "decorationrolevisitor.h"

#include <coreplugin/fileiconprovider.h>

#include <asn1/definitions.h>
#include <asn1/file.h>
#include <asn1/project.h>
#include <asn1/typeassignment.h>
#include <asn1/typereference.h>
#include <asn1/valueassignment.h>

#include <utils/icon.h>
#include <utils/stylehelper.h>

using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal;

QIcon Asn1Acn::Internal::TreeViews::createIcon(const QString &iconFile, Utils::Theme::Color color)
{
    if (iconFile.isEmpty()) {
        return QIcon();
    }

    return QIcon(Utils::Icon({{iconFile, color}}, Utils::Icon::Tint).icon());
}

QIcon Asn1Acn::Internal::TreeViews::typeIcon(const QString &iconFile)
{
    return createIcon(iconFile, Utils::Theme::IconsCodeModelStructColor);
}

QIcon Asn1Acn::Internal::TreeViews::valueIcon(const QString &iconFile)
{
    return createIcon(iconFile, Utils::Theme::IconsCodeModelVariableColor);
}

DecorationRoleVisitor::~DecorationRoleVisitor() {}

QIcon DecorationRoleVisitor::valueFor(const Definitions &defs) const
{
    Q_UNUSED(defs);
    const static QIcon icon(Utils::Icon({{QLatin1String(":/utils/images/namespace.png"),
                                          Utils::Theme::IconsCodeModelKeywordColor}},
                                        Utils::Icon::Tint)
                                .icon());
    return icon;
}

QIcon DecorationRoleVisitor::valueFor(const Asn1Acn::File &file) const
{
    Q_UNUSED(file);
    return Core::FileIconProvider::icon(QFileIconProvider::File);
}

QIcon DecorationRoleVisitor::valueFor(const Asn1Acn::TypeAssignment &type) const
{
    return typeIcon(type.type()->baseIconFile());
}

QIcon DecorationRoleVisitor::valueFor(const Asn1Acn::ValueAssignment &value) const
{
    return valueIcon(value.type()->baseIconFile());
}

QIcon DecorationRoleVisitor::valueFor(const Asn1Acn::Project &project) const
{
    Q_UNUSED(project);
    return Core::FileIconProvider::icon(QFileIconProvider::Folder);
}

QIcon DecorationRoleVisitor::valueFor(const Asn1Acn::Root &root) const
{
    Q_UNUSED(root);
    return {};
}
