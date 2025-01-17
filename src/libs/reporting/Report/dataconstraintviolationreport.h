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

#include <QList>
#include <QMetaType>
#include <QString>
#include <QVariant>

namespace reporting {

/**
 * @brief   Contains parsed information from an error identified as a data constraint violation.
 */
struct DataConstraintViolationReport {
    /** Name of the function */
    QString functionName;
    /** Name of the constraint violating variable */
    QString variableName;
    /** Name of the nested state */
    QString nestedStateName;
    /** Boundary operator (less than, greater than etc.) */
    QList<QString> constraints;
    /** Bounding value */
    QList<QVariant> boundingValues;
};

}

// Add meta type for QVariant compatibility
Q_DECLARE_METATYPE(reporting::DataConstraintViolationReport)
