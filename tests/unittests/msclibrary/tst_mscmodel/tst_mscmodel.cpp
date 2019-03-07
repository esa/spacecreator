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

private:
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

QTEST_APPLESS_MAIN(tst_MscModel)

#include "tst_mscmodel.moc"
