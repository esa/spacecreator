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

#include "definitions.h"
#include "exceptions.h"
#include "file.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmessagedeclaration.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "typeassignment.h"
#include "types/builtintypes.h"

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
    void testMessageCompliance();
    void testAllMessagesCompliance();
    void testAllDocuments();
    void testAllCharts();

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

void tst_MscModel::testMessageCompliance()
{
    auto doc = new MscDocument("Doc01", m_model);
    m_model->addDocument(doc);
    auto chart = new MscChart("Chart", m_model);
    doc->addChart(chart);
    auto inst = new MscInstance("Instance01", chart);
    chart->addInstance(inst);
    auto message = new MscMessage("sendTo", inst, nullptr, chart);
    message->setSourceInstance(inst);
    MscParameterList parameters;
    parameters.append(MscParameter("5"));
    message->setParameters(parameters);
    chart->addInstanceEvent(message, { { inst, -1 } });

    // Check without declarations - defaults to "ok"
    bool ok = m_model->checkMessageAsn1Compliance(*message);
    QCOMPARE(ok, true);

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

    // No matching declaration, is "ok" as well
    parameters.clear();
    parameters.append(MscParameter("5"));
    message->setParameters(parameters);
    message->setName("ReceiveFrom");

    ok = m_model->checkMessageAsn1Compliance(*message);
    QCOMPARE(ok, true);
}

void tst_MscModel::testAllMessagesCompliance()
{
    auto doc1 = new MscDocument("Doc01", m_model);
    m_model->addDocument(doc1);

    auto declaration = new MscMessageDeclaration;
    declaration->setNames({ "sendTo" });
    declaration->setTypeRefList({ "MyInt" });
    doc1->messageDeclarations()->append(declaration);

    auto chart1 = new MscChart("Chart01", m_model);
    doc1->addChart(chart1);
    auto inst1 = new MscInstance("Instance01", chart1);
    chart1->addInstance(inst1);
    auto message1 = new MscMessage("sendTo", chart1);
    message1->setSourceInstance(inst1);
    MscParameterList parameters;
    parameters.append(MscParameter("5"));
    message1->setParameters(parameters);
    chart1->addInstanceEvent(message1, { { inst1, -1 } });

    auto doc2 = new MscDocument("Doc02", m_model);
    m_model->addDocument(doc2);
    auto chart2 = new MscChart("Chart02", m_model);
    doc2->addChart(chart2);
    auto inst2 = new MscInstance("Instance02", chart2);
    chart2->addInstance(inst2);
    auto message2 = new MscMessage("sendTo", chart2);
    message2->setSourceInstance(inst2);
    MscParameterList parameters2;
    parameters2.append(MscParameter("8"));
    message2->setParameters(parameters2);
    chart2->addInstanceEvent(message2, { { inst2, -1 } });

    addAsn1Types();
    bool ok = m_model->checkAllMessagesForAsn1Compliance();
    QCOMPARE(ok, true);

    parameters2.clear();
    parameters2.append(MscParameter("foo"));
    message2->setParameters(parameters2);
    ok = m_model->checkAllMessagesForAsn1Compliance();
    QCOMPARE(ok, false);
}

void tst_MscModel::testAllDocuments()
{
    auto doc1 = new MscDocument("Doc01", m_model);
    m_model->addDocument(doc1);

    auto doc2 = new MscDocument("Doc02", doc1);
    doc1->addDocument(doc2);
    auto doc21 = new MscDocument("Doc021", doc1);
    doc1->addDocument(doc21);

    auto doc3 = new MscDocument("Doc03", doc2);
    doc2->addDocument(doc3);

    QVector<msc::MscDocument *> documents = m_model->allDocuments();
    QCOMPARE(documents.size(), 4);
}

void tst_MscModel::testAllCharts()
{
    auto chart0 = new MscChart("Chart0", m_model);
    m_model->addChart(chart0);

    auto doc1 = new MscDocument("Doc01", m_model);
    m_model->addDocument(doc1);
    auto chart1 = new MscChart("Chart01", doc1);
    doc1->addChart(chart1);
    auto chart11 = new MscChart("Chart01", doc1);
    doc1->addChart(chart11);

    auto doc2 = new MscDocument("Doc02", doc1);
    doc1->addDocument(doc2);
    auto chart2 = new MscChart("Chart02", doc2);
    doc2->addChart(chart2);

    QVector<msc::MscChart *> charts = m_model->allCharts();
    QCOMPARE(charts.size(), 4);
}

void tst_MscModel::addAsn1Types()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Integer>();
    type->setParameters({ { "min", 0 }, { "max", 255 } });
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyInt", location, std::move(type));

    auto asn1Definitions = std::make_unique<Asn1Acn::Definitions>("TestDef", location);
    asn1Definitions->addType(std::move(assignment));
    auto asn1Data = QSharedPointer<Asn1Acn::File>().create("/dumm/path");
    asn1Data->add(std::move(asn1Definitions));
    m_model->setAsn1TypesData(asn1Data);
}

QTEST_APPLESS_MAIN(tst_MscModel)

#include "tst_mscmodel.moc"
