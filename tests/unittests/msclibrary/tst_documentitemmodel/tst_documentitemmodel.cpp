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

#include <exceptions.h>
#include <documentitemmodel.h>
#include <mscmodel.h>

#include <mscchart.h>
#include <mscdocument.h>

#include <QtTest>

using namespace msc;

class tst_DocumentItemModel : public QObject
{
    Q_OBJECT

    void fillDocument();

private Q_SLOTS:
    void init();
    void cleanup();
    void testColumnCount();
    void testMscRowCount();
    void testRowCount();
    void testIndex();
    void testParent();
    void testIndexForModel();

private:
    DocumentItemModel *m_document = nullptr;
    MscModel *m_model = nullptr;
};

void tst_DocumentItemModel::fillDocument()
{
    Q_ASSERT(m_document);
    Q_ASSERT(m_model);

    auto doc1 = new MscDocument("Level 1", m_model);
    m_model->addDocument(doc1);

    auto doc2 = new MscDocument("Level 2", doc1);
    doc1->addDocument(doc2);

    doc2->addChart(new MscChart("Level 2 - Chart 1", doc2));
    doc2->addChart(new MscChart("Level 2 - Chart 2", doc2));
}

void tst_DocumentItemModel::init()
{
    m_document = new DocumentItemModel();
    m_model = new MscModel();
    m_document->setMscModel(m_model);
    fillDocument();
}

void tst_DocumentItemModel::cleanup()
{
    delete m_document;
    m_document = nullptr;
    delete m_model;
    m_model = nullptr;
}

void tst_DocumentItemModel::testColumnCount()
{
    QCOMPARE(m_document->columnCount(QModelIndex()), 1);
}

void tst_DocumentItemModel::testMscRowCount()
{
    m_model->clear();
    QCOMPARE(m_document->rowCount(QModelIndex()), 0);
    m_model->addChart(new MscChart(m_model));
    QCOMPARE(m_document->rowCount(QModelIndex()), 1);
}

void tst_DocumentItemModel::testRowCount()
{
    QCOMPARE(m_document->rowCount(QModelIndex()), 1);

    QModelIndex idx1 = m_document->index(0, 0, QModelIndex());
    QCOMPARE(m_document->rowCount(idx1), 1);

    QModelIndex idx2 = m_document->index(0, 0, idx1);
    QCOMPARE(m_document->rowCount(idx2), 2);
}

void tst_DocumentItemModel::testIndex()
{
    QModelIndex idx1 = m_document->index(0, 0, QModelIndex());
    auto doc1 = static_cast<MscDocument *>(idx1.internalPointer());
    QCOMPARE(doc1->name(), QString("Level 1"));

    QModelIndex idx2 = m_document->index(0, 0, idx1);
    auto doc2 = static_cast<MscDocument *>(idx2.internalPointer());
    QCOMPARE(doc2->name(), QString("Level 2"));

    // check the charts
    QModelIndex idx21 = m_document->index(0, 0, idx2);
    auto chart21 = static_cast<MscChart *>(idx21.internalPointer());
    QCOMPARE(chart21->name(), QString("Level 2 - Chart 1"));

    QModelIndex idx22 = m_document->index(1, 0, idx2);
    auto chart22 = static_cast<MscChart *>(idx22.internalPointer());
    QCOMPARE(chart22->name(), QString("Level 2 - Chart 2"));
}

void tst_DocumentItemModel::testParent()
{
    QCOMPARE(m_document->parent(QModelIndex()).isValid(), false);

    QModelIndex idx1 = m_document->index(0, 0, QModelIndex());
    QModelIndex parent1 = m_document->parent(idx1);
    QCOMPARE(parent1.isValid(), false);

    QModelIndex idx2 = m_document->index(0, 0, idx1);
    QModelIndex parent2 = m_document->parent(idx2);
    QCOMPARE(parent2.internalPointer(), idx1.internalPointer());

    // check the charts
    QModelIndex idx21 = m_document->index(0, 0, idx2);
    QModelIndex parent21 = m_document->parent(idx21);
    QCOMPARE(parent21.internalPointer(), idx2.internalPointer());

    QModelIndex idx22 = m_document->index(1, 0, idx2);
    QModelIndex parent22 = m_document->parent(idx22);
    QCOMPARE(parent22.internalPointer(), idx2.internalPointer());
}

void tst_DocumentItemModel::testIndexForModel()
{
    QModelIndex idx1 = m_document->index(0, 0, QModelIndex());
    QModelIndex idx2 = m_document->index(0, 0, idx1);
    QModelIndex idx21 = m_document->index(0, 0, idx2);

    MscChart *chart1 = m_document->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QModelIndex chart1Index = m_document->index(chart1);
    QCOMPARE(chart1Index, idx21);

    QModelIndex idx22 = m_document->index(1, 0, idx2);
    MscChart *chart2 = m_document->mscModel()->documents().at(0)->documents().at(0)->charts().at(1);
    QModelIndex chart2Index = m_document->index(chart2);
    QCOMPARE(chart2Index, idx22);

    QModelIndex index = m_document->index(nullptr);
    QCOMPARE(index.isValid(), false);
}

QTEST_APPLESS_MAIN(tst_DocumentItemModel)

#include "tst_documentitemmodel.moc"
