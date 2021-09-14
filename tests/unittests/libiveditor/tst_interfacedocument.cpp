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

#include "abstractproject.h"
#include "asn1modelstorage.h"
#include "asn1systemchecks.h"
#include "interfacedocument.h"
#include "iveditor.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivtestutils.h"

#include <QObject>
#include <QtTest>
#include <memory>

class IvProjectMoc : public shared::AbstractProject
{
public:
    explicit IvProjectMoc(QObject *parent = nullptr)
        : shared::AbstractProject(parent)
    {
    }

    QStringList allDVFiles() const override { return {}; }
    QStringList allIVFiles() const override { return {}; }
    QStringList allMscFiles() const override { return {}; }
    QStringList allAsn1Files() const override { return { { QString(EXAMPLES_DIR) + "asn1/dataview-uniq.asn" } }; }
};

class tst_InterfaceDocument : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void test_checkAllInterfacesForAsn1Compliance();
    void test_loadFailingFunctionInit();

private:
    std::unique_ptr<ive::InterfaceDocument> m_ivDoc;
    std::unique_ptr<IvProjectMoc> m_project;
    std::unique_ptr<Asn1Acn::Asn1ModelStorage> m_asnStorage;
    std::unique_ptr<Asn1Acn::Asn1SystemChecks> m_asnCheck;
};

void tst_InterfaceDocument::initTestCase()
{
    ivm::initIVLibrary();
    ive::initIVEditor();
    QStandardPaths::setTestModeEnabled(true);
}

void tst_InterfaceDocument::init()
{
    m_asnStorage = std::make_unique<Asn1Acn::Asn1ModelStorage>();
    m_project = std::make_unique<IvProjectMoc>();
    m_asnCheck = std::make_unique<Asn1Acn::Asn1SystemChecks>();
    m_asnCheck->setAsn1Storage(m_asnStorage.get());
    m_asnCheck->setProject(m_project.get());

    m_ivDoc = std::make_unique<ive::InterfaceDocument>();
    m_ivDoc->setAsn1Check(m_asnCheck.get());
}

void tst_InterfaceDocument::test_checkAllInterfacesForAsn1Compliance()
{
    m_ivDoc->setPath(QString(EXAMPLES_DIR).append("asn1/interfaceview.xml"));
    m_ivDoc->setAsn1FileName("dataview-uniq.asn");

    auto fn1 = ivm::testutils::createFunction("Fn1");
    m_ivDoc->objectsModel()->addObject(fn1);

    // Empty parameters results to true
    auto if1 = ivm::testutils::createIface(fn1, ivm::IVInterface::InterfaceType::Provided, "If1");
    bool ok = m_ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, true);

    // Used type is defined in ASN1
    auto if2 = ivm::testutils::createIface(fn1, ivm::IVInterface::InterfaceType::Provided, "If2");
    if2->addParam(shared::InterfaceParameter("IfaceParam", shared::BasicParameter::Type::Other, "T-UInt32"));
    ok = m_ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, true);

    // Unknown type
    auto if3 = ivm::testutils::createIface(fn1, ivm::IVInterface::InterfaceType::Provided, "If3");
    if3->addParam(shared::InterfaceParameter("IfaceParam", shared::BasicParameter::Type::Other, "InvalidType"));
    ok = m_ivDoc->checkAllInterfacesForAsn1Compliance();
    QCOMPARE(ok, false);
}

void tst_InterfaceDocument::test_loadFailingFunctionInit()
{
    m_ivDoc->load(QFINDTESTDATA("interfaceview_function_fail.xml"));
}

QTEST_MAIN(tst_InterfaceDocument)

#include "tst_interfacedocument.moc"
