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

#include <QObject>
#include <QString>
#include "ivobject.h"

namespace ivm {

/**
* @brief IVArchetypeReference reference for archetype function in function type
* It is used to specify which archetype functions are implemented in specific function type
*/
class IVArchetypeReference : public IVObject
{
    Q_OBJECT

public:
    explicit IVArchetypeReference(QObject *parent = nullptr, const QString &archetypeLibraryName = QString(),
            const QString &archetypeFunctionName = QString());
    ~IVArchetypeReference() = default;

    QString getLibraryName() const;
    void setLibraryName(const QString &libraryName);
    QString getFunctionName() const;
    void setFunctionName(const QString &functionName);
    bool operator==(const IVArchetypeReference &other) const;

private:
    QString m_archetypeLibraryName;
    QString m_archetypeFunctionName;
};

}
