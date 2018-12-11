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

class MscElementImpl : public MscElement
{
    Q_OBJECT
public:
    explicit MscElementImpl(QObject *parent = nullptr)
        : MscElement(parent)
    {
    }

    MscElementImpl(const QString &name, QObject *parent = nullptr)
        : MscElement(name, parent)
    {
    }

    chart::Element elementType() const override
    {
        return chart::Element::Chart; // doesn't matter here
    }
};

class tst_MscElement : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDefaultName();
    void testConstructorName();
    void testSetName();

private:
    static const QString TestElementName;
};

const QString tst_MscElement::TestElementName = QString("Roselileo:)");

void tst_MscElement::testDefaultName()
{
    MscElementImpl element;
    QCOMPARE(element.name(), MscElement::DefaultName);
}

void tst_MscElement::testConstructorName()
{
    MscElementImpl element(TestElementName);
    QCOMPARE(element.name(), TestElementName);
}

void tst_MscElement::testSetName()
{
    MscElementImpl element;
    QSignalSpy spy(&element, &MscElement::nameChanged);
    element.setName(TestElementName);
    QCOMPARE(element.name(), TestElementName);
    QCOMPARE(spy.count(), 1);
}

QTEST_APPLESS_MAIN(tst_MscElement)

#include "tst_mscelement.moc"
