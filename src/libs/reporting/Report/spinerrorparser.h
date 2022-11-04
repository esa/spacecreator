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
#include "observerfailurereport.h"
#include "rawerroritem.h"
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
     * @brief   Parse from multiple spin errors
     *
     * @param   spinMessages     Spin command outputs
     * @param   spinTraces       Spin traces
     * @param   sclConditions    SCL file conditions
     * @param   scenario         scenario.sim file contents
     *
     * @return  List of spin errors
     */
    SpinErrorReport parse(const QStringList &spinMessages, const QStringList &sclConditionFiles,
            const QList<RawErrorItem> &parameters, const QStringList &observerNames) const;

    /**
     * @brief   Parse from multiple spin errors
     * @param   rawErrors        List of raw error messages
     * @return  List of spin errors
     */
    SpinErrorReport parse(const QList<RawErrorItem> rawErrors) const;

    /**
     * @brief   Parse from single spin error
     * @param   rawErrors        List of raw error messages
     * @return  List of spin errors
     */
    SpinErrorReport parse(const RawErrorItem &rawError) const;

private:
    static const QString m_spinNoTrailFileMessage;
    static const QHash<QString, StopConditionViolationReport::ViolationType> m_stopConditionViolationIdentifiers;

    SpinErrorReportItem parseSpinTraces(const QString &spinTraces, const QStringList &sclConditions) const;

    QRegularExpressionMatch matchAcceptanceCycle(const QString &spinTraces) const;
    QRegularExpressionMatch matchVariableViolation(const QString &spinTraces) const;

    SpinErrorReportItem buildDataConstraintViolationReportItem(const QRegularExpressionMatch &matchedError) const;
    SpinErrorReportItem buildStopConditionViolationReportItem(const QString &conditions) const;

    QRegularExpressionMatchIterator matchSpinErrors(const QString &spinMessage) const;
    QVariant parseVariableViolation(const QString &rawError) const;
    QVariant parseStopConditionViolation(const QString &rawError) const;
    QVariant parseObserverFailureErrorState() const;
    QVariant parseObserverFailureSuccessState() const;

    static QString readFile(const QString &filePath);

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
