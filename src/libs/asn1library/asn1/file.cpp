/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a program and funded by
** European Space Agency.
**
** This Library is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "file.h"

#include "asn1valueparser.h"
#include "mutatingvisitor.h"
#include "visitor.h"

using namespace Asn1Acn;

File::File(const QString &path)
    : Node(path, { path, 0, 0 })
    , m_polluted(false)
{
}

File::File(const File &other)
    : File(other.location().path())
{
    for (const auto &def : other.definitionsList())
        add(std::make_unique<Definitions>(*def));
    for (const auto &ref : other.references())
        addTypeReference(std::make_unique<TypeReference>(*ref));
    for (const auto &err : other.errors())
        addErrorMessage(err);
    if (other.isPolluted())
        setPolluted();
}

File::~File() {}

void File::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

void File::accept(MutatingVisitor &visitor)
{
    visitor.visit(*this);
}

const Definitions *File::definitions(const QString &name) const
{
    const auto it = m_definitionsByNameMap.find(name);
    return it != m_definitionsByNameMap.end() ? it->second : nullptr;
}

Definitions *File::definitions(const QString &name)
{
    auto it = m_definitionsByNameMap.find(name);
    return it != m_definitionsByNameMap.end() ? it->second : nullptr;
}

void File::add(std::unique_ptr<Definitions> defs)
{
    defs->setParent(this);
    m_definitionsByNameMap[defs->name()] = defs.get();
    m_definitionsList.push_back(std::move(defs));
}

void File::addTypeReference(std::unique_ptr<TypeReference> ref)
{
    m_referencesMap.insert(std::make_pair(ref->location().line(), ref.get()));
    m_references.push_back(std::move(ref));
}

void File::addErrorMessage(const ErrorMessage &message)
{
    m_errorList.push_back(message);
}

void File::clearReferences()
{
    m_referencesMap.clear();
    m_references.clear();
}

void File::clearErrors()
{
    m_errorList.clear();
}

/*!
   Returns the first type assignment with the given name/identifier
 */
const std::unique_ptr<TypeAssignment> &File::typeAssignment(const QString &text) const
{
    for (const std::unique_ptr<Definitions> &def : m_definitionsList) {
        for (const std::unique_ptr<TypeAssignment> &assign : def->types()) {
            if (text == assign->name()) {
                return assign;
            }
        }
    }
    return m_noType;
}

/*!
   Returns the first type having the given \p name
 */
const Types::Type *File::typeFromName(const QString &name) const
{
    for (const std::unique_ptr<Asn1Acn::Definitions> &definitions : m_definitionsList) {
        for (const std::unique_ptr<Asn1Acn::TypeAssignment> &assignment : definitions->types()) {
            if (assignment->name() == name) {
                return assignment->type();
            }
        }
    }
    return nullptr;
}

/*!
   Returns if the type \p typeName exists
 */
bool File::hasType(const QString &typeName) const
{
    return typeFromName(typeName) != nullptr;
}

/*!
   Check if the \p parameter of type \p typeName is valid by this ASN1 file
 */
bool File::checkAsn1Compliance(const QString &parameter, const QString &typeName) const
{
    for (const std::unique_ptr<Asn1Acn::Definitions> &definitions : m_definitionsList) {
        for (const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssignment : definitions->types()) {
            if (typeAssignment->name() == typeName) {
                bool ok;
                Asn1Acn::Asn1ValueParser parser;
                parser.parseAsn1Value(typeAssignment.get(), parameter, &ok);
                if (ok) {
                    return true;
                }
            }
        }
    }

    return false;
}
