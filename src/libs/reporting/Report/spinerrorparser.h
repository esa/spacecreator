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

#include "dataconstraintviolationreport.h"
#include "spinerrorreportitem.h"

class QRegularExpressionMatch;
class QRegularExpressionMatchIterator;

namespace reporting {

/**
 * @brief   Parses and generates error reports based on spin messages.
 */
class SpinErrorParser
{
public:
    enum ConstraintViolationParseTokens
    {
        ConstraintViolationVariableName = 1,
        ConstraintViolationType,
        ConstraintViolationBoundingValue
    };

    enum ReportParseTokens
    {
        ErrorNumber = 1,
        ErrorType,
        ErrorDetails,
        ErrorDepth
    };
    /**
     * @brief   Constructor
     */
    SpinErrorParser() = default;

    /**
     * @brief   Parse spin message
     *
     * @param   spinMessage      Spin command output
     *
     * @return  List of spin errors
     */
    SpinErrorReport parse(const QString &spinMessage) const;

private:
    QRegularExpressionMatchIterator findSpinErrors(const QString &spinMessage) const;
    QList<QVariant> findVariableViolations(const QString &variableViolation) const;

    SpinErrorReportItem buildReport(const QRegularExpressionMatch &matchedError) const;

    static QRegularExpression buildSpinErrorRegex();
    static QRegularExpression buildDataConstraintViolationRegex();
};

}
