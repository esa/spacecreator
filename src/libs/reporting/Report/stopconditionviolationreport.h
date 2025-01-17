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

#include <QMetaType>
#include <QString>

namespace reporting {

/**
 * @brief   Contains parsed information from an error identified as a stop condition violation.
 */
struct StopConditionViolationReport {
    /// @brief  Violation clause
    enum ViolationClause
    {
        Never = 0,
        Always,
        Eventually,
        FilterOut,
        UnknownClause
    };

    /// @brief  Violation type
    enum ViolationType
    {
        Empty = 0,
        Exist,
        GetState,
        Length,
        QueueLast,
        QueueLength,
        Present,
        UndefinedType
    };

    /** Violation clause */
    ViolationClause violationClause;
    /** Violation type */
    ViolationType violationType;
};

}

// Add meta type for QVariant compatibility
Q_DECLARE_METATYPE(reporting::StopConditionViolationReport)
