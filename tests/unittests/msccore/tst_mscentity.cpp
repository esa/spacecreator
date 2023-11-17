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

#include "cif/cifblockfactory.h"
#include "cif/ciflines.h"
#include "mscentity.h"

#include <QSignalSpy>
#include <QtTest>

using namespace msc;

class MscEntitytImpl : public MscEntity
{
public:
    explicit MscEntitytImpl(QObject *parent = nullptr)
        : MscEntity(parent)
    {
    }

    MscEntitytImpl(const QString &name, QObject *parent = nullptr)
        : MscEntity(name, parent)
    {
    }

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
    void testExtractingRequirementFromCif();

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

void tst_MscEntity::testExtractingRequirementFromCif()
{
    MscEntitytImpl entity;

    cif::CifBlockShared requirementCif =
            cif::CifBlockFactory::createBlock({ cif::CifLineShared(new cif::CifLineRequirement()) });
    QByteArray req { "a3b4-1f2e,a9f3-742d" };
    requirementCif->setPayload(QVariant::fromValue(req), cif::CifLine::CifType::Requirement);

    entity.setCifs({ requirementCif });

    QByteArrayList expected { "a3b4-1f2e", "a9f3-742d" };
    QCOMPARE(entity.requirements(), expected);
    QCOMPARE(entity.cifs().size(), 1);

    entity.setRequirements({});
    QCOMPARE(entity.requirements(), QByteArrayList());
    QCOMPARE(entity.cifs().size(), 0);
}

QTEST_APPLESS_MAIN(tst_MscEntity)

#include "tst_mscentity.moc"
