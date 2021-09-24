/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "datatype.h"

#include <QString>
#include <optional>

namespace tmc::promelamodel {
class Declaration final
{
public:
    enum Visibility
    {
        NORMAL,
        SHOW,
        HIDDEN
    };

public:
    Declaration(DataType type, QString name, Visibility visibility = Visibility::NORMAL);
    const DataType &getType() const;
    const QString &getName() const;
    Visibility getVisibility() const;

private:
    DataType m_type;
    QString m_name;
    Visibility m_visibility;
    // std::optional<Expr> init
};
}
