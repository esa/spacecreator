/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "common.h"

#include <QtTest>

class tst_Common : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testReadFilesList();
};

void tst_Common::testReadFilesList()
{
    const QStringList filePaths = shared::readFilesList(QFINDTESTDATA("asn1files.pro"), { "acn", "asn", "asn1" });
    QCOMPARE(filePaths.size(), 12);
    QCOMPARE(filePaths.filter("_multiline.").size(), 4);
    QCOMPARE(filePaths.filter("_singleline.").size(), 4);
    QVERIFY(filePaths.filter("$$PWD").isEmpty());
    QVERIFY(filePaths.filter("interfaceview").isEmpty());
    QCOMPARE(filePaths.filter(".acn").size(), 5);
    QCOMPARE(filePaths.filter(".asn1").size(), 3);
    QCOMPARE(filePaths.filter(".xml").size(), 0);
}

QTEST_MAIN(tst_Common)

#include "tst_common.moc"
