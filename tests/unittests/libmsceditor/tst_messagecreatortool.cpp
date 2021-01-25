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

#include "msccreate.h"
#include "mscinstance.h"
#include "tools/messagecreatortool.h"

#include <QtTest>
#include <memory>

class tst_MessageCreatorTool : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testReCreate();
    void testViceVersaCreate();
    void testCyclicCreate();
    void testCreateToGlobal();
    void testCreateFromGlobal();
};

void tst_MessageCreatorTool::testReCreate()
{
    auto instanceA = std::make_unique<msc::MscInstance>();
    auto instanceB = std::make_unique<msc::MscInstance>();

    // inst A creates B
    auto create1 = std::make_unique<msc::MscCreate>();
    create1->setSourceInstance(instanceA.get());
    create1->setTargetInstance(instanceB.get());
    instanceB->setExplicitCreator(instanceA.get());

    // inst A creates B (again)
    auto create2 = std::make_unique<msc::MscCreate>();
    create2->setSourceInstance(instanceA.get());
    create2->setTargetInstance(instanceB.get());

    msc::MessageCreatorTool tool(msc::MscMessage::MessageType::Create, nullptr, nullptr);
    QCOMPARE(tool.validateCreate(create2.get()), false);
}

void tst_MessageCreatorTool::testViceVersaCreate()
{
    auto instanceA = std::make_unique<msc::MscInstance>();
    auto instanceB = std::make_unique<msc::MscInstance>();

    // inst A creates B
    auto create1 = std::make_unique<msc::MscCreate>();
    create1->setSourceInstance(instanceA.get());
    create1->setTargetInstance(instanceB.get());
    instanceB->setExplicitCreator(instanceA.get());

    // inst B creates A
    auto create2 = std::make_unique<msc::MscCreate>();
    create2->setSourceInstance(instanceB.get());
    create2->setTargetInstance(instanceA.get());

    msc::MessageCreatorTool tool(msc::MscMessage::MessageType::Create, nullptr, nullptr);
    QCOMPARE(tool.validateCreate(create2.get()), false);
}

void tst_MessageCreatorTool::testCyclicCreate()
{
    auto instanceA = std::make_unique<msc::MscInstance>();
    auto instanceB = std::make_unique<msc::MscInstance>();
    auto instanceC = std::make_unique<msc::MscInstance>();

    // inst A creates B
    auto create1 = std::make_unique<msc::MscCreate>();
    create1->setSourceInstance(instanceA.get());
    create1->setTargetInstance(instanceB.get());
    instanceB->setExplicitCreator(instanceA.get());

    // inst B creates C
    auto create2 = std::make_unique<msc::MscCreate>();
    create2->setSourceInstance(instanceB.get());
    create2->setTargetInstance(instanceC.get());
    instanceC->setExplicitCreator(instanceB.get());

    // inst C creates A
    auto create3 = std::make_unique<msc::MscCreate>();
    create3->setSourceInstance(instanceC.get());
    create3->setTargetInstance(instanceA.get());

    msc::MessageCreatorTool tool(msc::MscMessage::MessageType::Create, nullptr, nullptr);
    QCOMPARE(tool.validateCreate(create3.get()), false);
}

void tst_MessageCreatorTool::testCreateToGlobal()
{
    auto instanceA = std::make_unique<msc::MscInstance>();

    // inst A created from outside
    auto create1 = std::make_unique<msc::MscCreate>();
    create1->setTargetInstance(instanceA.get());

    msc::MessageCreatorTool tool(msc::MscMessage::MessageType::Create, nullptr, nullptr);
    QCOMPARE(tool.validateCreate(create1.get()), false);
}

void tst_MessageCreatorTool::testCreateFromGlobal()
{
    auto instanceA = std::make_unique<msc::MscInstance>();

    // inst A creates to outside
    auto create1 = std::make_unique<msc::MscCreate>();
    create1->setSourceInstance(instanceA.get());

    msc::MessageCreatorTool tool(msc::MscMessage::MessageType::Create, nullptr, nullptr);
    QCOMPARE(tool.validateCreate(create1.get()), false);
}

QTEST_MAIN(tst_MessageCreatorTool)

#include "tst_messagecreatortool.moc"
