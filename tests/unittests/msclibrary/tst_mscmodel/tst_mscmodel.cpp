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

#include "exceptions.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmessagedeclaration.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"

#include <QtTest>

using namespace msc;

class tst_MscModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testAddDocument();
    void DocumentInstance();
    void testNoNullPtrDocument();
    void testAddChart();
    void testNoDuplicateChart();
    void testNoNullPtrChart();
    void testAsn1Compliance();
    void testMessageCompliance();

private:
    void addAsn1Types();
    MscModel *m_model = nullptr;
};

void tst_MscModel::init()
{
    m_model = new MscModel;
}

void tst_MscModel::cleanup()
{
    delete m_model;
    m_model = nullptr;
}

void tst_MscModel::testAddDocument()
{
    QCOMPARE(m_model->documents().size(), 0);

    auto doc1 = new MscDocument("Doc01", m_model);
    m_model->addDocument(doc1);
    auto doc2 = new MscDocument("Doc02", m_model);
    m_model->addDocument(doc2);
    QCOMPARE(m_model->documents().size(), 2);
}

void tst_MscModel::DocumentInstance()
{
    auto doc = new MscDocument("Doc", m_model);
    m_model->addDocument(doc);
    m_model->addDocument(doc);
    QCOMPARE(m_model->documents().size(), 1);
}

void tst_MscModel::testNoNullPtrDocument()
{
    m_model->addDocument(nullptr);
    QCOMPARE(m_model->documents().size(), 0);
}

void tst_MscModel::testAddChart()
{
    QCOMPARE(m_model->charts().size(), 0);

    auto doc1 = new MscChart("Chart01", m_model);
    m_model->addChart(doc1);
    auto doc2 = new MscChart("Chart02", m_model);
    m_model->addChart(doc2);
    QCOMPARE(m_model->charts().size(), 2);
}

void tst_MscModel::testNoDuplicateChart()
{
    auto chart = new MscChart("Chart", m_model);
    m_model->addChart(chart);
    m_model->addChart(chart);
    QCOMPARE(m_model->charts().size(), 1);
}

void tst_MscModel::testNoNullPtrChart()
{
    m_model->addChart(nullptr);
    QCOMPARE(m_model->charts().size(), 0);
}

void tst_MscModel::testAsn1Compliance()
{
    m_model->setAsn1TypesData({});
    bool ok = m_model->checkparameterAsn1Compliance("", "MyInt");
    QCOMPARE(ok, true);

    addAsn1Types();

    ok = m_model->checkparameterAsn1Compliance("5", "MyInt");
    QCOMPARE(ok, true);

    ok = m_model->checkparameterAsn1Compliance("false", "MyInt");
    QCOMPARE(ok, false);
}

void tst_MscModel::testMessageCompliance()
{
    auto doc = new MscDocument("Doc01", m_model);
    m_model->addDocument(doc);
    auto chart = new MscChart("Chart", m_model);
    doc->addChart(chart);
    auto inst = new MscInstance("Instance01", chart);
    chart->addInstance(inst);
    auto message = new MscMessage("sendTo", chart);
    message->setSourceInstance(inst);
    MscParameterList parameters;
    parameters.append(MscParameter("5"));
    message->setParameters(parameters);
    chart->addInstanceEvent(message);

    bool ok = m_model->checkMessageAsn1Compliance(*message);
    QCOMPARE(ok, false);

    addAsn1Types();
    // add mesage declaration
    auto declaration = new MscMessageDeclaration;
    declaration->setNames({ "sendTo" });
    declaration->setTypeRefList({ "MyInt" });
    doc->messageDeclarations()->append(declaration);

    ok = m_model->checkMessageAsn1Compliance(*message);
    QCOMPARE(ok, true);

    // number of parmaters differ
    parameters.append(MscParameter("foo"));
    message->setParameters(parameters);
    ok = m_model->checkMessageAsn1Compliance(*message);
    QCOMPARE(ok, false);

    // No matching declaration
    parameters.clear();
    parameters.append(MscParameter("5"));
    message->setParameters(parameters);
    message->setName("ReceiveFrom");

    ok = m_model->checkMessageAsn1Compliance(*message);
    QCOMPARE(ok, false);
}

void tst_MscModel::addAsn1Types()
{
    QVariantList types;
    QVariantMap myInt;
    myInt["name"] = "MyInt";
    myInt["type"] = 0;
    myInt["isOptional"] = false;
    myInt["min"] = 0;
    myInt["max"] = 255;
    types.append(myInt);
    m_model->setAsn1TypesData(types);
}

QTEST_APPLESS_MAIN(tst_MscModel)

#include "tst_mscmodel.moc"
