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

#include "cif/ciflines.h"
#include "cifcommon/ciflinepointstestbase.h"

#include <QtTest>

using namespace msc::cif;

class tst_CifLineMessage : public CifLinePointsTestBase
{
    Q_OBJECT
private Q_SLOTS:
    void testDefault();
    void testInitFromValidSeparatorComma();
    void testInitFromValidSeparatorSpace();
    void testInitFromValidSeparatorCommaSpace();
    void testInitFromValidSeparatorNo();
    void testInitFromInsufficient();
    void testToString();

private:
    int m_expectedPointsAmount = 0;
    virtual CifLineShared createCifLine() const;
    virtual int expectedPointsAmount() const;
};

int tst_CifLineMessage::expectedPointsAmount() const
{
    if (const CifLineShared &line = createCifLine())
        if (CifLinePointsHolder *pointsHolder = dynamic_cast<CifLinePointsHolder *>(line.data()))
            return pointsHolder->targetPointsCount();

    return CifLinePointsHolder::AllPoints; // TODO: replace with QFAIL?
}

CifLineShared tst_CifLineMessage::createCifLine() const
{
    return CifLineShared(new CifLineMessage);
}

void tst_CifLineMessage::testDefault()
{
    checkDefault();
}

void tst_CifLineMessage::testInitFromValidSeparatorComma()
{
    for (m_expectedPointsAmount = 1; m_expectedPointsAmount <= 10; ++m_expectedPointsAmount)
        checkInitFromValid(generateTestLine(m_expectedPointsAmount, m_separatorComma));
}

void tst_CifLineMessage::testInitFromValidSeparatorSpace()
{
    for (m_expectedPointsAmount = 1; m_expectedPointsAmount <= 10; ++m_expectedPointsAmount)
        checkInitFromValid(generateTestLine(m_expectedPointsAmount, m_separatorSpace));
}

void tst_CifLineMessage::testInitFromValidSeparatorCommaSpace()
{
    for (m_expectedPointsAmount = 1; m_expectedPointsAmount <= 10; ++m_expectedPointsAmount)
        checkInitFromValid(generateTestLine(m_expectedPointsAmount, m_separatorCommaSpace));
}

void tst_CifLineMessage::testInitFromValidSeparatorNo()
{
    for (m_expectedPointsAmount = 1; m_expectedPointsAmount <= 10; ++m_expectedPointsAmount)
        checkInitFromValid(generateTestLine(m_expectedPointsAmount, m_separatorNone));
}

void tst_CifLineMessage::testInitFromInsufficient()
{
    m_expectedPointsAmount = 1;
    checkInitFromInsufficient();
}

void tst_CifLineMessage::testToString()
{
    checkToString();
}

QTEST_APPLESS_MAIN(tst_CifLineMessage)

#include "tst_ciflinemessage.moc"
