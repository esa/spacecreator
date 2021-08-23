/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "connectioncreationvalidator.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvmodel.h"
#include "dvnode.h"

#include <QObject>
#include <QTest>

class tst_DVConnectionUtils : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void tst_createValidation();

private:
    dvm::DVModel *model;
};

void tst_DVConnectionUtils::initTestCase()
{
    model = new dvm::DVModel;
}

void tst_DVConnectionUtils::cleanupTestCase()
{
    delete model;
    model = nullptr;
}

void tst_DVConnectionUtils::tst_createValidation()
{
    auto n1 = new dvm::DVNode();
    n1->setTitle(QLatin1String("Node1"));

    auto d1 = new dvm::DVDevice(n1);
    d1->setTitle(QLatin1String("Device1"));
    QString attrName = dvm::meta::Props::token(dvm::meta::Props::Token::port);
    d1->setEntityAttribute(attrName, QLatin1String("PortName"));
    attrName = dvm::meta::Props::token(dvm::meta::Props::Token::requires_bus_access);
    d1->setEntityAttribute(attrName, QLatin1String("BusName"));

    n1->addDevice(d1);

    auto n2 = new dvm::DVNode();
    n2->setTitle(QLatin1String("Node2"));

    auto d2 = new dvm::DVDevice(n2);
    d2->setTitle(QLatin1String("Device2"));
    attrName = dvm::meta::Props::token(dvm::meta::Props::Token::port);
    d2->setEntityAttribute(attrName, QLatin1String("PortName"));
    attrName = dvm::meta::Props::token(dvm::meta::Props::Token::requires_bus_access);
    d2->setEntityAttribute(attrName, QLatin1String("BusName"));

    n2->addDevice(d2);

    dvm::ConnectionCreationValidator::FailReason reason =
            dvm::ConnectionCreationValidator::canConnect(nullptr, nullptr, nullptr, nullptr);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::NoStartDevice);

    reason = dvm::ConnectionCreationValidator::canConnect(nullptr, nullptr, d1, nullptr);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::NoEndDevice);

    reason = dvm::ConnectionCreationValidator::canConnect(nullptr, nullptr, d1, d2);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::NoStartNode);

    reason = dvm::ConnectionCreationValidator::canConnect(n1, nullptr, d1, d2);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::NoEndNode);

    reason = dvm::ConnectionCreationValidator::canConnect(n1, n1, d1, d2);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::SameParent);

    reason = dvm::ConnectionCreationValidator::canConnect(n1, n2, d1, d1);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::SameDevice);

    reason = dvm::ConnectionCreationValidator::canConnect(n1, n2, d1, d2);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::NoScene);

    model->addObject(n1);
    model->addObject(d1);
    model->addObject(n2);
    model->addObject(d2);

    reason = dvm::ConnectionCreationValidator::canConnect(n1, n2, d1, d2);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::NotFail);

    auto c = new dvm::DVConnection(d1, d2);
    QVERIFY(c != nullptr);
    model->addObject(c);

    reason = dvm::ConnectionCreationValidator::canConnect(n1, n2, d1, d2);
    QVERIFY(reason == dvm::ConnectionCreationValidator::FailReason::AlreadyExists);
}

QTEST_MAIN(tst_DVConnectionUtils)

#include "tst_dvconnectionutils.moc"
