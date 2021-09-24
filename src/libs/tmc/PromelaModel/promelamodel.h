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

#include "namedmtype.h"
#include "utype.h"

#include <QList>
#include <QSet>
#include <QString>
#include <conversion/common/model.h>
#include <map>

namespace tmc::promelamodel {
class PromelaModel final : public conversion::Model
{
public:
    PromelaModel();
    ~PromelaModel() override;
    auto modelType() const -> conversion::ModelType override;

    void addMtype(const QString &value);
    const QSet<QString> &getMtypeValues() const;

    void addNamedMtype(const NamedMtype &mtype);
    const std::map<QString, NamedMtype> &getNamedMtypeValues() const;

    void addUtype(const Utype &utype);
    const QList<Utype> &getUtypes() const;

private:
    QSet<QString> m_mtype_values;
    std::map<QString, NamedMtype> m_named_m_type_values;
    QList<Utype> m_user_types;
    std::map<QString, Declaration> m_declarations;

    // global variables
};
}
