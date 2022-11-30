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
#include "mscfailurereport.h"
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
    enum ViolationTypeParseTokens
    {
        RawErrorMatch = 0,
        ErrorDetailsMatch = 1
    };

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

    /// @brief  Regex match token for MSC observer parsing
    enum MscObserverParseTokens
    {
        ObserverName = 1
    };

    /**
     * @brief   Constructor
     */
    SpinErrorParser() = default;

    /**
     * @brief   Parse from multiple spin errors
     *
     * @param   spinMessages     Spin command outputs
     * @param   sclFiles         SCL condition files
     * @param   errors           Raw error data
     * @param   mscObserverFiles MSC observer files
     *
     * @return  List of spin errors
     */
    SpinErrorReport parse(const QStringList &spinMessages, const QStringList &sclFiles,
            const QList<RawErrorItem> &errors, const QStringList &mscObserverFiles) const;

private:
    static const QString m_spinNoTrailFileMessage;
    static const QHash<QString, StopConditionViolationReport::ViolationClause> m_stopConditionViolationClauses;
    static const QHash<QString, StopConditionViolationReport::ViolationType> m_stopConditionViolationTypes;

    SpinErrorReportItem parseTrace(
            const QString &spinTraces, const QStringList &sclConditions, const QStringList &mscObservers) const;

    QRegularExpressionMatch matchStopCondition(const QString &spinTraces) const;
    QRegularExpressionMatch matchObserverFailureErrorState(const QString &spinTraces) const;
    QRegularExpressionMatch matchObserverFailureSuccessState(const QString &spinTraces) const;
    QRegularExpressionMatch matchVariableViolation(const QString &spinTraces) const;

    SpinErrorReportItem buildDataConstraintViolationReportItem(const QRegularExpressionMatch &matchedError) const;
    SpinErrorReportItem buildStopConditionViolationReportItem(const QString &conditions) const;

    QVariant parseVariableViolation(const QString &parsedErrorToken) const;
    QVariant parseStopConditionViolation(const QString &parsedErrorToken) const;
    QVariant parseObserverFailureErrorState(const QString &parsedErrorToken) const;
    QVariant parseObserverFailureSuccessState(const QString &parsedErrorToken) const;
    QVariant parseMscFailure(const QString &parsedErrorToken) const;

    static QString readFile(const QString &filePath);

    static QRegularExpression buildDataConstraintViolationRegex();
    static QRegularExpression buildStopConditionViolationRegex();
    static QRegularExpression buildStopConditionExpressionRegex();

    static QString removeParentheses(const QString &numberToken);
    static QString cleanUpSclComments(const QString &scl);
    static QStringList splitExpression(const QString &expression);
    static StopConditionViolationReport::ViolationType resolveViolationType(const QStringList &expressions);

    static void parseVariableName(const QString &variable, DataConstraintViolationReport &violationReport);
    static QStringList parseMscObserver(const QString &mscFileText);
};

}
