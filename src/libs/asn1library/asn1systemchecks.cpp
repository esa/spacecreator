/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "asn1systemchecks.h"

#include "abstractproject.h"
#include "asn1/definitions.h"
#include "asn1modelstorage.h"

namespace Asn1Acn {

Asn1SystemChecks::Asn1SystemChecks(QObject *parent)
    : QObject(parent)
{
}

void Asn1SystemChecks::setAsn1Storage(Asn1Acn::Asn1ModelStorage *storage)
{
    m_storage = storage;
}

Asn1ModelStorage *Asn1SystemChecks::asn1Storage() const
{
    return m_storage;
}

void Asn1SystemChecks::setProject(shared::AbstractProject *project)
{
    m_project = project;
}

QStringList Asn1SystemChecks::allTypeNames() const
{
    if (!m_project || !m_storage) {
        return {};
    }

    QStringList typeNames;
    for (const QString &fileName : m_project->allAsn1Files()) {
        QSharedPointer<Asn1Acn::File> asn1File = m_storage->asn1DataTypes(fileName);
        if (asn1File) {
            for (const std::unique_ptr<Asn1Acn::Definitions> &definitions : asn1File->definitionsList()) {
                for (const std::unique_ptr<Asn1Acn::TypeAssignment> &type : definitions->types()) {
                    typeNames << type->name();
                }
            }
        }
    }
    return typeNames;
}

/*!
   All ASN1 definitions of all files in one vector
 */
std::vector<Definitions *> Asn1SystemChecks::definitionsList() const
{
    if (!m_project || !m_storage) {
        return {};
    }

    std::vector<Definitions *> defs;
    for (const QString &fileName : m_project->allAsn1Files()) {
        QSharedPointer<Asn1Acn::File> asn1File = m_storage->asn1DataTypes(fileName);
        if (asn1File) {
            const File::DefinitionsList &list = asn1File->definitionsList();
            for (const std::unique_ptr<Definitions> &d : list) {
                defs.push_back(d.get());
            }
        }
    }
    return defs;
}

bool Asn1SystemChecks::checkAsn1Compliance(const QString &parameter, const QString &typeName) const
{
    if (!m_project || !m_storage) {
        return false;
    }

    for (const QString &fileName : m_project->allAsn1Files()) {
        QSharedPointer<Asn1Acn::File> asn1File = m_storage->asn1DataTypes(fileName);
        if (asn1File) {
            if (asn1File->checkAsn1Compliance(parameter, typeName)) {
                return true;
            }
        }
    }
    return false;
}

/*!
   Returns the first type assignment with the given name/identifier
 */
const std::unique_ptr<TypeAssignment> &Asn1SystemChecks::typeAssignment(const QString &text) const
{
    if (!m_project || !m_storage) {
        return m_noType;
    }

    for (const QString &fileName : m_project->allAsn1Files()) {
        QSharedPointer<Asn1Acn::File> asn1File = m_storage->asn1DataTypes(fileName);
        if (asn1File) {
            const std::unique_ptr<TypeAssignment> &type = asn1File->typeAssignment(text);
            if (type) {
                return type;
            }
        }
    }
    return m_noType;
}

/*!
   Returns the first type having the given \p name
 */
const Types::Type *Asn1SystemChecks::typeFromName(const QString &name) const
{
    if (!m_project || !m_storage) {
        return nullptr;
    }

    for (const QString &fileName : m_project->allAsn1Files()) {
        QSharedPointer<Asn1Acn::File> asn1File = m_storage->asn1DataTypes(fileName);
        if (asn1File) {
            const Types::Type *type = asn1File->typeFromName(name);
            if (type) {
                return type;
            }
        }
    }
    return nullptr;
}

bool Asn1SystemChecks::hasType(const QString &name) const
{
    if (!m_project || !m_storage) {
        return false;
    }
    for (const QString &fileName : m_project->allAsn1Files()) {
        QSharedPointer<Asn1Acn::File> asn1File = m_storage->asn1DataTypes(fileName);
        if (asn1File) {
            if (asn1File->hasType(name)) {
                return true;
            }
        }
    }
    return false;
}

} // namespace Asn1Acn
