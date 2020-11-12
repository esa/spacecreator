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
#include "importfindingvisitor.h"

#include <asn1/definitions.h>
#include <asn1/file.h>
#include <asn1/project.h>
#include <asn1/root.h>

using namespace Asn1Acn::Internal::Completion;

ImportFindingVisitor::ImportFindingVisitor(const QString &module, const QString &import)
    : m_module(module)
    , m_import(import)
{}

Asn1Acn::Node *ImportFindingVisitor::valueFor(const Asn1Acn::Root &root) const
{
    for (const auto &project : root.projects()) {
        const auto res = valueFor(*project);
        if (res != nullptr)
            return res;
    }
    return nullptr;
}

Asn1Acn::Node *ImportFindingVisitor::valueFor(const Asn1Acn::Definitions &defs) const
{
    if (defs.name() != m_module)
        return nullptr;
    return isValue() ? findImportByName(defs.values()) : findImportByName(defs.types());
}

bool ImportFindingVisitor::isValue() const
{
    return m_import.size() > 0 && m_import[0].isLower();
}

template<typename Collection>
Asn1Acn::Node *ImportFindingVisitor::findImportByName(const Collection &col) const
{
    for (const auto &element : col)
        if (element->name() == m_import)
            return element.get();
    return nullptr;
}

Asn1Acn::Node *ImportFindingVisitor::valueFor(const Asn1Acn::File &file) const
{
    const auto defs = file.definitions(m_module);
    return defs == nullptr ? nullptr : valueFor(*defs);
}

Asn1Acn::Node *ImportFindingVisitor::valueFor(const Asn1Acn::TypeAssignment &type) const
{
    Q_UNUSED(type);
    return nullptr;
}

Asn1Acn::Node *ImportFindingVisitor::valueFor(const Asn1Acn::ValueAssignment &value) const
{
    Q_UNUSED(value);
    return nullptr;
}

Asn1Acn::Node *ImportFindingVisitor::valueFor(const Asn1Acn::Project &project) const
{
    for (const auto &file : project.files()) {
        const auto res = valueFor(*file);
        if (res != nullptr)
            return res;
    }
    return nullptr;
}
