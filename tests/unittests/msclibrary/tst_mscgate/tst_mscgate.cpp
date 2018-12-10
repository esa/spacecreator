/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QtTest>

#include <mscgate.h>

class tst_MscGate : public QObject
{
    Q_OBJECT

private slots:
    void test_stub();
};

void tst_MscGate::test_stub()
{
    QSKIP(qPrintable(QString(
            "\n\tThis is just an initial stub for Gate testing,"
            "\n\twill be populated afterwhile.")));
}

QTEST_APPLESS_MAIN(tst_MscGate)

#include "tst_mscgate.moc"
