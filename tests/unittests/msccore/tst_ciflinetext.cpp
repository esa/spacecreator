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

class tst_CifLineText : public CifLinePointsTestBase
{
    Q_OBJECT
private Q_SLOTS:
    void testDefault();
    void testInitFromValidSeparatorComma();
    void testInitFromValidSeparatorSpace();
    void testInitFromValidSeparatorCommaSpace();
    void testInitFromValidSeparatorNo();
    void testInitFromInsufficient();
    void testInitFromExcessive();
    void testToString();

    void testSetText();

private:
    virtual CifLineShared createCifLine() const;
};

CifLineShared tst_CifLineText::createCifLine() const
{
    return CifLineShared(new CifLineText);
}

void tst_CifLineText::testDefault()
{
    checkDefault();
}

void tst_CifLineText::testInitFromValidSeparatorComma()
{
    checkInitFromValidSeparatorComma();
}

void tst_CifLineText::testInitFromValidSeparatorSpace()
{
    checkInitFromValidSeparatorSpace();
}

void tst_CifLineText::testInitFromValidSeparatorCommaSpace()
{
    checkInitFromValidSeparatorCommaSpace();
}

void tst_CifLineText::testInitFromValidSeparatorNo()
{
    CifLinePointsTestBase::checkInitFromValidSeparatorNo();
}

void tst_CifLineText::testInitFromInsufficient()
{
    checkInitFromInsufficient();
}

void tst_CifLineText::testInitFromExcessive()
{
    checkInitFromExcessive();
}

void tst_CifLineText::testToString()
{
    checkToString();
}

void tst_CifLineText::testSetText()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
}

QTEST_APPLESS_MAIN(tst_CifLineText)

#include "tst_ciflinetext.moc"
