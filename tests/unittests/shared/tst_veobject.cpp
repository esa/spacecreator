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

#include "veobject.h"

#include <QtTest>

class XEObject : public shared::VEObject
{

    // VEObject interface
public:
    QString title() const override { return {}; }
    QString titleUI() const override { return {}; }
    QVector<qint32> coordinates() const override { return {}; }
    void setCoordinates(const QVector<qint32> &coordinates) override { Q_UNUSED(coordinates) }
    bool postInit() override { return false; }
    bool aboutToBeRemoved() override { return false; }
    QVariantList generateProperties(bool isProperty) const override
    {
        Q_UNUSED(isProperty)
        return {};
    }
};

class tst_VEObject : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSetAttributeImpl();
};

void tst_VEObject::testSetAttributeImpl()
{
    XEObject obj;
    const QString attrName = "foo";
    obj.setEntityAttribute(attrName, QVariant(42));
    QCOMPARE(obj.entityAttribute(attrName).isExportable(), true);

    obj.setAttributeExportable(attrName, false);
    QCOMPARE(obj.entityAttribute(attrName).isExportable(), false);

    obj.setEntityAttribute(attrName, QVariant(43));
    QCOMPARE(obj.entityAttribute(attrName).isExportable(), false);
}

QTEST_MAIN(tst_VEObject)

#include "tst_veobject.moc"
