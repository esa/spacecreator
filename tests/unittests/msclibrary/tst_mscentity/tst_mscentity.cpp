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

#include <mscentity.h>

#include <QtTest>
#include <QSignalSpy>

using namespace msc;

class MscEntitytImpl : public MscEntity
{
public:
    explicit MscEntitytImpl(QObject *parent = nullptr) : MscEntity(parent) {}

    MscEntitytImpl(const QString &name, QObject *parent = nullptr) : MscEntity(name, parent) {}

    MscEntity::EntityType entityType() const override
    {
        return MscEntity::EntityType::Chart; // doesn't matter here
    }
};

class tst_MscEntity : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDefaultName();
    void testConstructorName();
    void testSetName();

private:
    static const QString TestEntityName;
};

const QString tst_MscEntity::TestEntityName = QString("Roselileo:)");

void tst_MscEntity::testDefaultName()
{
    MscEntitytImpl entity;
    QCOMPARE(entity.name(), MscEntity::DefaultName);
}

void tst_MscEntity::testConstructorName()
{
    MscEntitytImpl entity(TestEntityName);
    QCOMPARE(entity.name(), TestEntityName);
}

void tst_MscEntity::testSetName()
{
    MscEntitytImpl entity;
    QSignalSpy spy(&entity, &MscEntity::nameChanged);
    entity.setName(TestEntityName);
    QCOMPARE(entity.name(), TestEntityName);
    QCOMPARE(spy.count(), 1);
}

QTEST_APPLESS_MAIN(tst_MscEntity)

#include "tst_mscentity.moc"
