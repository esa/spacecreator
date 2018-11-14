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

#include <mscelement.h>

#include <QtTest>
#include <QSignalSpy>

using namespace msc;

class tst_MscElement : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDefaultName();
    void testConstructorName();
    void testSetName();
};

void tst_MscElement::testDefaultName()
{
    MscElement element;
    QCOMPARE(element.name(), QString("Untitled"));
}

void tst_MscElement::testConstructorName()
{
    const QString elementName("Rosetta");
    MscElement element(elementName);
    QCOMPARE(element.name(), elementName);
}

void tst_MscElement::testSetName()
{
    MscElement element;
    const QString elementName("Galileo");
    QSignalSpy spy(&element, &MscElement::nameChanged);
    element.setName(elementName);
    QCOMPARE(element.name(), elementName);
    QCOMPARE(spy.count(), 1);
}

QTEST_APPLESS_MAIN(tst_MscElement)

#include "tst_mscelement.moc"
