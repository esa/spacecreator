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

#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "aadltestutils.h"
#include "asn1library.h"
#include "asn1modelstorage.h"
#include "interface/interfacedocument.h"
#include "iveditor.h"

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
    std::unique_ptr<aadlinterface::InterfaceDocument> ivDoc;
};

void tst_InterfaceDocument::initTestCase()
{
    Asn1Acn::initAsn1Library();
    aadlinterface::initIvEditor();
    QStandardPaths::setTestModeEnabled(true);
}

void tst_InterfaceDocument::init()
{
    ivDoc = std::make_unique<aadlinterface::InterfaceDocument>();
}

void tst_InterfaceDocument::test_checkAllInterfacesForAsn1Compliance()
{
    // see "asn1library/asn1resources/taste-types.asn" for the default
    auto fn1 = new aadl::AADLObjectFunction("Fn1");
    ivDoc->objectsModel()->addObject(fn1);

    // Empty parameters results to true
    auto if1 = aadl::testutils::createIface(fn1, aadl::AADLObjectIface::IfaceType::Provided, "If1");
    ivDoc->objectsModel()->addObject(if1);
    bool ok = ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, true);

    // Used type is defined in ASN1
    auto if2 = aadl::testutils::createIface(fn1, aadl::AADLObjectIface::IfaceType::Provided, "If2");
    if2->addParam(aadl::IfaceParameter("IfaceParam", aadl::BasicParameter::Type::Other, "T-Int31"));
    ivDoc->objectsModel()->addObject(if2);
    ok = ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, true);

    // Unknown type
    auto if3 = aadl::testutils::createIface(fn1, aadl::AADLObjectIface::IfaceType::Provided, "If3");
    if3->addParam(aadl::IfaceParameter("IfaceParam", aadl::BasicParameter::Type::Other, "InvalidType"));
    ivDoc->objectsModel()->addObject(if3);
    ok = ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, false);
}

QTEST_MAIN(tst_InterfaceDocument)

#include "tst_interfacedocument.moc"
