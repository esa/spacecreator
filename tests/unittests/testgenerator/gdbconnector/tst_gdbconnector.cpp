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

#include "../common.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <qobjectdefs.h>

namespace tests::testgenerator {

class tst_gdbconnector final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testNominal();

private:
};

void tst_gdbconnector::initTestCase()
{
    //
}

void tst_gdbconnector::testNominal()
{
    QFAIL("this shall happen");

    // to check:
    // 1. load a given script file
    // 2. execute script on a specified console application
    // 3. retrieve the returned result
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_gdbconnector)

#include "tst_gdbconnector.moc"
