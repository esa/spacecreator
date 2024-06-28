/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2024 N7 Space Sp. z o.o.
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

#include "helper.h"

#include <QHash>
#include <QString>
#include <asn1library/asn1/asn1model.h>
#include <promela/PromelaModel/printfstatement.h>
#include <set>

namespace promela::translator {
class CCodeGenerator
{
public:
    CCodeGenerator(const Asn1Acn::Asn1Model *asn1Model, const std::set<QString> &messageTypes);

    QString assignmentFromPromelaToC(const QString &typeName, const QString &target, const QString &source);
    QString assignmentFromCToPromela(const QString &typeName, const QString &target, const QString &source);
    QList<promela::model::PrintfStatement> printfStatements(
            const QString &typeName, const QString &variable, const QString &additionalMessage);

private:
    QHash<QString, QString> m_templatesFromPromelaToC;
    QHash<QString, QString> m_templatesFromCToPromela;
    QHash<QString, QList<Helper::PrintfTemplate>> m_printfTemplates;
};

}
