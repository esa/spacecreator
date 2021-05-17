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

#include "asn1modelstorage.h"
#include "interface/interfacedocument.h"
#include "iveditor.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivtestutils.h"

#include <QObject>
#include <QtTest>
#include <memory>

class tst_InterfaceDocument : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void test_checkAllInterfacesForAsn1Compliance();

private:
    std::unique_ptr<ive::InterfaceDocument> ivDoc;
};

void tst_InterfaceDocument::initTestCase()
{
    ivm::initIVLibrary();
    ive::initIVEditor();
    QStandardPaths::setTestModeEnabled(true);
}

void tst_InterfaceDocument::init()
{
    ivDoc = std::make_unique<ive::InterfaceDocument>();
}

void tst_InterfaceDocument::test_checkAllInterfacesForAsn1Compliance()
{
    ivDoc->setPath(QString(EXAMPLES_DIR).append("asn1/interfaceview.xml"));
    ivDoc->setAsn1FileName("dataview-uniq.asn");

    auto fn1 = new ivm::IVFunction("Fn1");
    ivDoc->objectsModel()->addObject(fn1);

    // Empty parameters results to true
    auto if1 = ivm::testutils::createIface(fn1, ivm::IVInterface::InterfaceType::Provided, "If1");
    bool ok = ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, true);

    // Used type is defined in ASN1
    auto if2 = ivm::testutils::createIface(fn1, ivm::IVInterface::InterfaceType::Provided, "If2");
    if2->addParam(ivm::InterfaceParameter("IfaceParam", ivm::BasicParameter::Type::Other, "T-UInt32"));
    ok = ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, true);

    // Unknown type
    auto if3 = ivm::testutils::createIface(fn1, ivm::IVInterface::InterfaceType::Provided, "If3");
    if3->addParam(ivm::InterfaceParameter("IfaceParam", ivm::BasicParameter::Type::Other, "InvalidType"));
    ok = ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, false);
}

QTEST_MAIN(tst_InterfaceDocument)

#include "tst_interfacedocument.moc"
