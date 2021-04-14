/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivfunction.h"
#include "ivlibrary.h"
#include "ivnamevalidator.h"

#include <QStandardPaths>
#include <QTest>

class tst_AADLNameValidator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test_functionName_data();
    void test_functionName();
};

void tst_AADLNameValidator::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    ivm::initIVLibrary();
}

void tst_AADLNameValidator::test_functionName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Simple name") << QString("Observer") << true;
    QTest::newRow("Has number") << QString("Observer7") << true;
    QTest::newRow("Underscore") << QString("Observer_Bingo") << true;
    QTest::newRow("Empty name") << QString("") << false;
    QTest::newRow("Special character") << QString("Object&Subscriber") << false;
    QTest::newRow("Start with number") << QString("2Observer") << false;
    QTest::newRow("Unicode") << QString("Obs\u00F6rver") << false;
    QTest::newRow("Underscore at start") << QString("_Observer") << false;
    QTest::newRow("Underscore at end") << QString("Observer_") << false;
    QTest::newRow("Reserved word 1") << QString("class") << false;
    QTest::newRow("Reserved word 2") << QString("struct") << false;
    QTest::newRow("Reserved word 3") << QString("static") << false;
    QTest::newRow("Reserved word 4") << QString("void") << false;
}

void tst_AADLNameValidator::test_functionName()
{
    QFETCH(QString, name);
    QFETCH(bool, expectedResult);

    const bool ok = ivm::IVNameValidator::isValidName(name);
    QCOMPARE(ok, expectedResult);
}

QTEST_APPLESS_MAIN(tst_AADLNameValidator)

#include "tst_aadlnamevalidator.moc"
