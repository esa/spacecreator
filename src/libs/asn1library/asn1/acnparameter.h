/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a programme and funded by
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

#include <QString>
#include <memory>
#include <vector>

namespace Asn1Acn {

class AcnParameter
{
public:
    AcnParameter();
    AcnParameter(const QString &id, const QString &name, const QString &type)
        : m_id(id)
        , m_name(name)
        , m_type(type)
    {
    }

    AcnParameter(const AcnParameter &other) = default;

    const QString &id() const { return m_id; }
    const QString &name() const { return m_name; }
    const QString &type() const { return m_type; }

private:
    QString m_id;
    QString m_name;
    QString m_type;
};

using AcnParameterPtr = std::unique_ptr<AcnParameter>;
using AcnParameterPtrs = std::vector<AcnParameterPtr>;

}
