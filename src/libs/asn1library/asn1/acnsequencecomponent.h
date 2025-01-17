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
#include "types/type.h"

#include <QString>
#include <memory>
#include <optional>
#include <vector>

namespace Asn1Acn {

class AcnSequenceComponent : public SequenceComponent
{
public:
    AcnSequenceComponent() = default;
    ~AcnSequenceComponent() override = default;

    AcnSequenceComponent(const QString &id, const QString &name, std::unique_ptr<Types::Type> type);

    AcnSequenceComponent(const AcnSequenceComponent &other);

    std::unique_ptr<SequenceComponent> clone() const override;
    QString definitionAsString() const override;
    QString presentWhen() const override;

    void accept(SequenceComponentVisitor &visitor) override;

    const QString &id() const;

    void setReference(const QString &reference);
    std::optional<QString> reference() const;

private:
    QString m_id;
    std::optional<QString> m_reference;
};

using AcnSequenceComponentPtr = std::unique_ptr<AcnSequenceComponent>;

}
