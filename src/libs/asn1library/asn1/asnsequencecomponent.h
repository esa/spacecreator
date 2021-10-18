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

#include "sequencecomponent.h"

#include <optional>

namespace Asn1Acn {

class AsnSequenceComponent : public SequenceComponent
{
public:
    AsnSequenceComponent() = default;
    ~AsnSequenceComponent() override = default;

    AsnSequenceComponent(const QString &name, const QString &cName, bool optional, std::optional<QString> defaultValue,
            const QString &presentWhen, const SourceLocation &location, std::unique_ptr<Types::Type> type);

    AsnSequenceComponent(const AsnSequenceComponent &other);

    std::unique_ptr<SequenceComponent> clone() const override;
    QString definitionAsString() const override;
    QString presentWhen() const override;
    bool isOptional() const;
    const std::optional<QString> &defaultValue() const;

    void accept(SequenceComponentVisitor &visitor) override;

    const SourceLocation &location() const;

private:
    bool m_optional;
    std::optional<QString> m_defaultValue;
    QString m_presentWhen;
    SourceLocation m_location;
};

}
