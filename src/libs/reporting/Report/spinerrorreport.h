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
#include <QString>
#include <QVariant>

namespace reporting {

/**
 * @brief   Structure to hold parsed spin report data.
 */
struct SpinErrorReport {
    /** Error types */
    enum ErrorType
    {
        DataConstraintViolation,
        StopConditionViolation,
        ObserverFailure,
        OtherError
    };

    /** Spin error number, counted from 1 up */
    uint32_t errorNumber;
    /** Error depth reported by spin */
    uint32_t errorDepth;
    /** Error type reported by the message */
    ErrorType errorType;
    /** Raw error details reported by the message */
    QString rawErrorDetails;
    /** Parsed error details */
    QList<QVariant> parsedErrorDetails;
};

}
