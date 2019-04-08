/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "ciflinepointstestbase.h"

#include <QtTest>
#include <mscentity.h>

using namespace msc::cif;

const QString CifLinePointsTestBase::m_separatorCommaSpace = QString(", ");
const QString CifLinePointsTestBase::m_separatorComma = QString(',');
const QString CifLinePointsTestBase::m_separatorSpace = QString(' ');
const QString CifLinePointsTestBase::m_separatorNone = QString();

CifLinePointsTestBase::CifLinePointsTestBase()
{
    msc::MscEntity::setCifEnabled(true);
}

QString CifLinePointsTestBase::lineTag() const
{
    auto detectName = [&](bool *detected) {
        QString name;
        if (CifLineShared line = createCifLine()) {
            name = CifLine::nameForType(line->lineType());
        } else {
            *detected = false;
            name = "Can't aquire CifLine for testing.";
        }
        return name;
    };

    bool detected(true);
    static const QString name = detectName(&detected);
    if (!detected) {
        qWarning() << name;
    }

    return name;
}

int CifLinePointsTestBase::expectedPointsAmount() const
{
    if (const CifLineShared &line = createCifLine())
        if (CifLinePointsHolder *pointsHolder = dynamic_cast<CifLinePointsHolder *>(line.data()))
            return pointsHolder->targetPointsCount();

    return CifLinePointsHolder::AllPoints; // TODO: replace with QFAIL?
}

QStringList CifLinePointsTestBase::generateTestLine(int pointsCount, const QString &separator) const
{
    static const QString lineTemplate("CIF %1 %2");
    auto fillPoints = [pointsCount, separator](int positiveness, bool isInt) {
        const QString pointTemplate(isInt ? "(%1, %1)" : "(%1.%1, %1.%1)");
        QString points;
        for (int i = 1; i <= pointsCount; ++i) {
            if (!separator.isEmpty() && !points.isEmpty())
                points.append(separator);
            points.append(pointTemplate.arg(i * positiveness));
        }
        return points;
    };

    const QString &lineTypeName = lineTag();
    const QString &positiveInts = lineTemplate.arg(lineTypeName, fillPoints(1, true));
    const QString &negativeInts = lineTemplate.arg(lineTypeName, fillPoints(-1, true));

    return { positiveInts, negativeInts };
}

void CifLinePointsTestBase::checkInitFromValid(const QStringList &sources)
{
    for (const QString &source : sources)
        if (CifLineShared line = createCifLine()) {
            const bool initialized = line->initFrom(source);
            QVERIFY(initialized == true);

            const QVector<QPoint> &points = line->payload().value<QVector<QPoint>>();

            const int expectedPointsCount = expectedPointsAmount();
            if (expectedPointsCount != CifLinePointsHolder::AllPoints)
                QCOMPARE(points.size(), expectedPointsCount);

            const int positiveness = sources.indexOf(source) % 2 == 0 ? 1 : -1;
            for (int i = 0; i < points.size(); ++i) {
                const QPoint &point = points.at(i);
                const int coordinate = (i + 1) * positiveness;
                QCOMPARE(point, QPoint(coordinate, coordinate));
            }
        }
}

void CifLinePointsTestBase::checkDefault()
{
    if (CifLineShared line = createCifLine()) {
        const QVector<QPoint> &points = line->payload().value<QVector<QPoint>>();
        QVERIFY(points.isEmpty());
    }
}

void CifLinePointsTestBase::checkInitFromValidSeparatorComma()
{
    checkInitFromValid(generateTestLine(expectedPointsAmount(), m_separatorComma));
}

void CifLinePointsTestBase::checkInitFromValidSeparatorSpace()
{
    checkInitFromValid(generateTestLine(expectedPointsAmount(), m_separatorSpace));
}

void CifLinePointsTestBase::checkInitFromValidSeparatorCommaSpace()
{
    checkInitFromValid(generateTestLine(expectedPointsAmount(), m_separatorCommaSpace));
}

void CifLinePointsTestBase::checkInitFromValidSeparatorNo()
{
    checkInitFromValid(generateTestLine(expectedPointsAmount(), m_separatorNone));
}

void CifLinePointsTestBase::checkInitFromInsufficient()
{
    if (CifLineShared line = createCifLine()) {
        const QStringList &sourceLines = generateTestLine(expectedPointsAmount() - 1, m_separatorCommaSpace);
        for (const QString &sourceLine : sourceLines) {
            const bool initialized = line->initFrom(sourceLine);
            QVERIFY(initialized == false);
        }
    } else {
        QFAIL("Can't aquire CifLine for testing.");
    }
}

void CifLinePointsTestBase::checkInitFromExcessive()
{
    if (CifLineShared line = createCifLine()) {
        const QStringList &sourceLines = generateTestLine(expectedPointsAmount() + 1, m_separatorCommaSpace);
        for (const QString &sourceLine : sourceLines) {
            const bool initialized = line->initFrom(sourceLine);
            QVERIFY(initialized == false);
        }
    } else {
        QFAIL("Can't aquire CifLine for testing.");
    }
}

void CifLinePointsTestBase::checkToString()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
}
