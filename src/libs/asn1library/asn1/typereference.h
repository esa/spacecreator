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
#pragma once

#include "sourcelocation.h"
#include <QString>

namespace Asn1Acn {

class TypeReference
{
public:
    TypeReference() = default;
    TypeReference(const QString &name, const QString &module, const SourceLocation &location = {})
        : m_location(location)
        , m_name(name)
        , m_module(module)
    {}
    TypeReference(const TypeReference &other) = default;

    const SourceLocation &location() const { return m_location; }
    const QString &name() const { return m_name; }
    const QString &module() const { return m_module; }

private:
    SourceLocation m_location;
    QString m_name;
    QString m_module;
};

}
