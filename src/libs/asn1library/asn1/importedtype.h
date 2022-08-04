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

#include <QHash>
#include <QString>

namespace Asn1Acn {

class ImportedType
{
public:
    ImportedType(const QString &module, const QString &name)
        : m_module(module)
        , m_name(name)
    {
        m_fullPath = QString("%1.%2").arg(m_module).arg(m_name);
    }

    const QString &module() const { return m_module; }
    const QString &name() const { return m_name; }
    const QString &fullPath() const { return m_fullPath; }

    bool operator<(const ImportedType &rhs) const { return m_fullPath < rhs.m_fullPath; }
    bool operator==(const ImportedType &rhs) const { return m_fullPath == rhs.m_fullPath; }
    bool operator!=(const ImportedType &rhs) const { return !(*this == rhs); }

private:
    QString m_module;
    QString m_name;
    QString m_fullPath;
};

}

namespace std {

template<>
struct hash<Asn1Acn::ImportedType> {
    std::size_t operator()(const Asn1Acn::ImportedType &it) const noexcept
    {
        return static_cast<size_t>(qHash(it.fullPath()));
    }
};

} // namespace std
