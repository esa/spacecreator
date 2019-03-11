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
#include "ciflinepointstestbase.h"

#include <QtTest>

using namespace msc::cif;

class tst_CifLineMscPageSize : public CifLinePointsTestBase
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

CifLineShared tst_CifLineMscPageSize::createCifLine() const
{
    return CifLineShared(new CifLineMscPageSize);
}

void tst_CifLineMscPageSize::testDefault()
{
    checkDefault();
}

void tst_CifLineMscPageSize::testInitFromValidSeparatorComma()
{
    checkInitFromValidSeparatorComma();
}

void tst_CifLineMscPageSize::testInitFromValidSeparatorSpace()
{
    checkInitFromValidSeparatorSpace();
}

void tst_CifLineMscPageSize::testInitFromValidSeparatorCommaSpace()
{
    checkInitFromValidSeparatorCommaSpace();
}

void tst_CifLineMscPageSize::testInitFromValidSeparatorNo()
{
    CifLinePointsTestBase::checkInitFromValidSeparatorNo();
}

void tst_CifLineMscPageSize::testInitFromInsufficient()
{
    checkInitFromInsufficient();
}

void tst_CifLineMscPageSize::testInitFromExcessive()
{
    checkInitFromExcessive();
}

void tst_CifLineMscPageSize::testToString()
{
    checkToString();
}

QTEST_APPLESS_MAIN(tst_CifLineMscPageSize)

#include "tst_ciflinemscpagesize.moc"
