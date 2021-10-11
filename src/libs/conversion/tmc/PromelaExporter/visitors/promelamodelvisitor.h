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

#include "tmc/PromelaModel/namedmtype.h"
#include "tmc/PromelaModel/promelamodel.h"
#include "tmc/PromelaModel/utype.h"

#include <QTextStream>

namespace conversion::tmc::exporter {
/**
 * @brief  Visitor for exporting @link{PromelaModel}
 *
 * This visitor exports PromelaModel to textual representation.
 */
class PromelaModelVisitor
{
public:
    /**
     * @brief Constructor.
     *
     * @param stream A stream to append exported model.
     */
    PromelaModelVisitor(QTextStream &stream);

    /**
     * @brief Visit PromelaModel
     *
     * @param promelaModel PromelaModel to visit.
     */
    void visit(const ::tmc::promela::model::PromelaModel &promelaModel);

private:
    void generateMtypes(const QSet<QString> &values);
    void generateNamedMtypes(const std::map<QString, ::tmc::promela::model::NamedMtype> &values);
    void generateMtypeNames(const QVector<QString> &names);
    void generateUtypes(const QList<::tmc::promela::model::Utype> &utypes);

private:
    QTextStream &m_stream;
};
}
