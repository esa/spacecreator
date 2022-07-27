/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include "archetypecommonprops.h"
#include "archetypeobject.h"
#include "parameterarchetype.h"

namespace ivm {

class InterfaceArchetype : public ArchetypeObject
{
    Q_OBJECT
public:
    enum class InterfaceType
    {
        PROVIDED = 0,
        REQUIRED
    };
    Q_ENUM(InterfaceType)

    enum class OperationKind
    {
        CYCLIC = 0,
        SPORADIC,
        PROTECTED,
        UNPROTECTED
    };
    Q_ENUM(OperationKind)

    explicit InterfaceArchetype(const QString &title, QObject *parent = nullptr,
            const InterfaceType &interfaceType = InterfaceType::PROVIDED,
            const OperationKind &operationKind = OperationKind::CYCLIC, const QString &layer = QString());
    ~InterfaceArchetype() override;

    InterfaceType getInterfaceType() const;
    void setInterfaceType(const InterfaceType &interfaceType);

    OperationKind getOperationKind() const;
    void setOperationKind(const OperationKind &operationKind);

    QString getLayer() const;
    void setLayer(const QString &layer);

    QVector<ParameterArchetype *> getParameters() const;
    void addParameter(ParameterArchetype *parameterArchetype);
    void removeParameter(ParameterArchetype *parameterArchetype);

    static QString interfaceTypeToString(const InterfaceType &interfaceType);
    static InterfaceType interfaceTypeFromString(
            const QString &interfaceTypeString, const InterfaceType &defaultInterfaceType = InterfaceType::PROVIDED);

    static QString operationKindToString(const OperationKind &operationKind);
    static OperationKind operationKindFromString(
            const QString &operationKindString, const OperationKind &defaultOperationKind = OperationKind::CYCLIC);

private:
    InterfaceArchetype::InterfaceType m_interfaceType;
    InterfaceArchetype::OperationKind m_operationKind;
    QVector<ParameterArchetype *> m_parameters;
};

}
