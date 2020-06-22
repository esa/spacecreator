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

#include "aadlobjectconnection.h"

#include <QtTest>

class tst_AADLObjectConnection : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testPostInitEmptyConnection();
};

void tst_AADLObjectConnection::testPostInitEmptyConnection()
{
    aadl::AADLObjectConnection connection(nullptr, nullptr, nullptr, nullptr);
    const bool result = connection.postInit();
    // postInit() for connections without source/target fails
    QCOMPARE(result, false);
}

QTEST_APPLESS_MAIN(tst_AADLObjectConnection)

#include "tst_aadlobjectconnection.moc"
