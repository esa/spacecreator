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

class tst_CifLineConcurrent : public CifLinePointsTestBase
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

private:
    virtual CifLineShared createCifLine() const;
};

CifLineShared tst_CifLineConcurrent::createCifLine() const
{
    return CifLineShared(new CifLineConcurrent);
}

void tst_CifLineConcurrent::testDefault()
{
    checkDefault();
}

void tst_CifLineConcurrent::testInitFromValidSeparatorComma()
{
    checkInitFromValidSeparatorComma();
}

void tst_CifLineConcurrent::testInitFromValidSeparatorSpace()
{
    checkInitFromValidSeparatorSpace();
}

void tst_CifLineConcurrent::testInitFromValidSeparatorCommaSpace()
{
    checkInitFromValidSeparatorCommaSpace();
}

void tst_CifLineConcurrent::testInitFromValidSeparatorNo()
{
    CifLinePointsTestBase::checkInitFromValidSeparatorNo();
}

void tst_CifLineConcurrent::testInitFromInsufficient()
{
    checkInitFromInsufficient();
}

void tst_CifLineConcurrent::testInitFromExcessive()
{
    checkInitFromExcessive();
}

void tst_CifLineConcurrent::testToString()
{
    checkToString();
}

QTEST_APPLESS_MAIN(tst_CifLineConcurrent)

#include "tst_ciflineconcurrent.moc"
