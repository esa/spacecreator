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
#include "stopconditionviolationreport.h"

class QRegularExpressionMatch;
class QRegularExpressionMatchIterator;

namespace reporting {

/**
 * @brief   Parses and generates error reports based on spin messages.
 */
class SpinErrorParser
{
public:
    /// @brief  Regex match tokens for constraint violation parsing
    enum ConstraintViolationParseTokens
    {
        ConstraintViolationVariableName = 1,
        ConstraintViolationType,
        ConstraintViolationBoundingValue
    };

    /// @brief  Regex match tokens for report item parsing
    enum ReportItemParseTokens
    {
        ErrorNumber = 1,
        ErrorType,
        ErrorDetails,
        ErrorDepth
    };

    /// @brief  Match tokens for nested variable name parsing
    enum VariableParseTokens
    {
        VariableGlobalState = 0,
        VariableFunction,
        VariableName
    };

    /// @brief  Regex match tokens for stop condition parsing
    enum StopConditionParseTokens
    {
        StopConditionClause = 1,
        StopConditionExpression
    };

    /// @brief  Regex match token for identifier parsing within stop condition
    enum IdentifierParseTokens
    {
        IdentifierName = 1
    };

    /**
     * @brief   Constructor
     */
    SpinErrorParser() = default;

    /**
     * @brief   Parse multiple spin messages
     *
     * @param   spinMessages     Spin command outputs
     * @param   spinTraces       Spin traces
     * @param   sclConditions    SCL file conditions
     *
     * @return  List of spin errors
     */
    SpinErrorReport parse(
            const QStringList &spinMessages, const QStringList &spinTraces, const QStringList &sclConditions) const;

    /**
     * @brief   Parse single spin message
     *
     * @param   spinMessage      Spin command output
     * @param   spinTraces       Spin traces
     * @param   sclConditions    SCL file conditions
     *
     * @return  List of spin errors
     */
    SpinErrorReport parse(const QString &spinMessage, const QString &spinTraces, const QString &sclConditions) const;

private:
    static const QString m_spinNoTrailFileMessage;
    static const QHash<QString, StopConditionViolationReport::ViolationType> m_stopConditionViolationIdentifiers;

    SpinErrorReportItem buildDataConstraintViolationReportItem(const QRegularExpressionMatch &matchedError) const;
    SpinErrorReportItem buildStopConditionViolationReportItem(const QString &conditions) const;

    QRegularExpressionMatchIterator matchSpinErrors(const QString &spinMessage) const;
    QVariant parseVariableViolation(const QString &rawError) const;
    QVariant parseStopConditionViolation(const QString &rawError) const;

    static QRegularExpression buildSpinErrorRegex();
    static QRegularExpression buildDataConstraintViolationRegex();

    static QRegularExpression buildStopConditionViolationRegex();
    static QRegularExpression buildStopConditionExpressionRegex();

    static QString cleanUpSclComments(const QString &scl);
    static QStringList splitExpression(const QString &expression);
    static StopConditionViolationReport::ViolationType resolveViolationType(const QStringList &expressions);

    static void parseVariableName(const QString &variable, DataConstraintViolationReport &violationReport);
};

}
