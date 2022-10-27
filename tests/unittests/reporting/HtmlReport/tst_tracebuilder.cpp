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

#include "tst_tracebuilder.h"

#include <reporting/HtmlReport/tracebuilder.h>

using namespace reporting;

namespace Report::test {

void tst_TraceBuilder::testSingleLineMessage()
{
    const QString message("Sample Message");
    const QString target("<span style=\"white-space: pre-wrap;\">Sample Message</span>");

    const TraceBuilder traceBuilder;
    const QString converted = traceBuilder.buildTraceReport(message);
    QVERIFY(converted == target);
}

void tst_TraceBuilder::testMultiLineMessage()
{
    const QString message("Sample\nMessage");
    const QString target("<span style=\"white-space: pre-wrap;\">Sample\n<br>Message</span>");

    const TraceBuilder traceBuilder;
    const QString converted = traceBuilder.buildTraceReport(message);
    QVERIFY(converted == target);
}

void tst_TraceBuilder::testHtmlEntities()
{
    const QString message("Sample & <b>Message</b>");
    const QString target("<span style=\"white-space: pre-wrap;\">Sample &amp; &lt;b&gt;Message&lt;/b&gt;</span>");

    const TraceBuilder traceBuilder;
    const QString converted = traceBuilder.buildTraceReport(message);
    QVERIFY(converted == target);
}

void tst_TraceBuilder::testTabulators()
{
    const QString message("Sample\t\tMessage");
    const QString target("<span style=\"white-space: pre-wrap;\">Sample&Tab;&Tab;Message</span>");

    const TraceBuilder traceBuilder;
    const QString converted = traceBuilder.buildTraceReport(message);
    QVERIFY(converted == target);
}

} // namespace Report::test
