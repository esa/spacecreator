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

#include "visitors/promelamodelvisitor.h"

#include "visitors/utypevisitor.h"

#include <QVector>
#include <algorithm>

namespace conversion::tmc::exporter {
using ::tmc::promela::model::NamedMtype;
using ::tmc::promela::model::PromelaModel;
using ::tmc::promela::model::Utype;

PromelaModelVisitor::PromelaModelVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void PromelaModelVisitor::visit(const PromelaModel &promelaModel)
{
    generateMtypes(promelaModel.getMtypeValues());
    generateNamedMtypes(promelaModel.getNamedMtypeValues());
    generateUtypes(promelaModel.getUtypes());
}

void PromelaModelVisitor::generateMtypes(const QSet<QString> &values)
{
    if (!values.isEmpty()) {
        m_stream << "mtype {\n";
        QVector<QString> mtype_values = QVector<QString>::fromList(values.values());
        std::sort(mtype_values.begin(), mtype_values.end());
        generateMtypeNames(mtype_values);
        m_stream << "}\n";
    }
}

void PromelaModelVisitor::generateNamedMtypes(const std::map<QString, NamedMtype> &values)
{
    for (auto iter = values.begin(); iter != values.end(); ++iter) {
        m_stream << "mtype :" << iter->first << " {\n";
        QVector<QString> mtype_values = QVector<QString>::fromList(iter->second.values().toList());
        std::sort(mtype_values.begin(), mtype_values.end());
        generateMtypeNames(mtype_values);
        m_stream << "}\n";
    }
}

void PromelaModelVisitor::generateMtypeNames(const QVector<QString> &names)
{
    for (const QString &value : names) {
        m_stream << "    " << value << ",\n";
    }
}

void PromelaModelVisitor::generateUtypes(const QList<Utype> &utypes)
{
    for (const Utype &utype : utypes) {
        UtypeVisitor visitor(m_stream);
        visitor.visit(utype);
    }
}

}
