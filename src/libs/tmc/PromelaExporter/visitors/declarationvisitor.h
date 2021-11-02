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

#include "tmc/PromelaModel/declaration.h"

#include <QTextStream>

namespace tmc::exporter {
/**
 * @brief  Visitor for exporting @link{::tmc::promelamodel::Declaration}
 */
class DeclarationVisitor
{
public:
    /**
     * @brief Constructor.
     *
     * @param stream  stream to append data type prefix
     * @param prefix  prefix, which shall be added at the beginning, e.g. indent
     */
    DeclarationVisitor(QTextStream &stream, QString prefix);

    /** @brief  Visit Declaration
     *
     * @param declaration Declaration to visit
     */
    void visit(const ::tmc::promela::model::Declaration &declaration);

private:
    QString getVisibilityString(const ::tmc::promela::model::Declaration &declaration);

private:
    QTextStream &m_stream;
    QString m_prefix;
};
}
