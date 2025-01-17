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

#include "sourcelocation.h"
#include "types/type.h"

#include <QString>
#include <memory>

namespace Asn1Acn {
class SequenceComponentVisitor;

class SequenceComponent
{
public:
    SequenceComponent() = default;
    virtual ~SequenceComponent() = default;

    SequenceComponent(const QString &name, const QString &cName, std::unique_ptr<Types::Type> type)
        : m_name(name)
        , m_cName(cName)
        , m_type(std::move(type))
    {
    }

    SequenceComponent(const SequenceComponent &other)
        : m_name(other.m_name)
        , m_cName(other.m_cName)
        , m_type(other.m_type->clone())
    {
    }

    virtual std::unique_ptr<SequenceComponent> clone() const = 0;
    virtual QString definitionAsString() const = 0;
    virtual QString presentWhen() const = 0;

    const std::vector<QString> &acnParameters() const { return m_asn1Parameters; }
    void addAcnParameter(QString acnParameter) { m_asn1Parameters.push_back(std::move(acnParameter)); }

    virtual void accept(SequenceComponentVisitor &visitor) = 0;

    const QString &name() const { return m_name; }
    const QString &cName() const { return m_cName; }

    const Types::Type *type() const { return m_type.get(); }
    Types::Type *type() { return m_type.get(); }

private:
    QString m_name;
    QString m_cName;
    std::unique_ptr<Types::Type> m_type;
    std::vector<QString> m_asn1Parameters;
};

}
