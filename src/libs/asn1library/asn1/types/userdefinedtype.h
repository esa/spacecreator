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

#include "type.h"
#include "../acnargument.h"

#include <vector>

#include <QString>

namespace Asn1Acn {
namespace Types {

class UserdefinedType : public Type
{
public:
    UserdefinedType(const QString &typeName, const QString &module);
    UserdefinedType(const UserdefinedType &other);

    QString typeName() const override;
    QString label() const override;
    ASN1Type typeEnum() const override;
    const QString &module() const;

    void accept(TypeMutatingVisitor &visitor) override;
    void accept(TypeReadingVisitor &visitor) const override;

    std::unique_ptr<Type> clone() const override;

    QString baseIconFile() const override;

    const Type *type() const { return m_type.get(); }
    Type *type() { return m_type.get(); }
    void setType(std::unique_ptr<Type> type);

    const AcnArgumentPtrs &acnArguments() const { return m_arguments; }
    void addArgument(AcnArgumentPtr argument);

private:
    QString m_name;
    QString m_module;

    std::unique_ptr<Type> m_type;
    AcnArgumentPtrs m_arguments;
};

}
}
