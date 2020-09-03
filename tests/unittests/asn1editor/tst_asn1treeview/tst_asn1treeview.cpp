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

#include "asn1treeview.h"
#include "asn1xmlparser.h"
#include "definitions.h"
#include "file.h"
#include "typeassignment.h"
#include "types/builtintypes.h"

#include <QtTest>

using namespace asn1;

class tst_Asn1TreeView : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testSetAsn1Value();

private:
    Asn1TreeView *m_treeView = nullptr;
};

void tst_Asn1TreeView::init()
{
    m_treeView = new Asn1TreeView;
}

void tst_Asn1TreeView::cleanup()
{
    delete m_treeView;
    m_treeView = nullptr;
}

void tst_Asn1TreeView::testSetAsn1Value()
{
    Asn1Acn::Asn1XMLParser parser;
    std::unique_ptr<Asn1Acn::File> types = parser.parseAsn1XmlFile(QFINDTESTDATA("DataView.xml"));

    const Asn1Acn::Definitions *definitions = types->definitions("DataView");
    QVERIFY(definitions != nullptr);
    const std::unique_ptr<Asn1Acn::TypeAssignment> &assignment = definitions->types().at(1);
    QVERIFY(assignment);

    m_treeView->setAsn1Model(assignment);

    QVariantMap subValue1;
    subValue1["name"] = QVariant::fromValue(QString("field-a"));
    subValue1["value"] = QVariant::fromValue(false);

    QVariantMap choiceMap;
    choiceMap["name"] = QVariant::fromValue(QString("choice1"));
    choiceMap["value"] = QVariant::fromValue(false);
    QVariantMap subValue2;
    subValue2["name"] = QVariant::fromValue(QString("field-b"));
    subValue2["choice"] = choiceMap;

    QVariantList value = { subValue1, subValue2 };

    QVariantMap asn1Value;
    asn1Value["children"] = value;
    asn1Value["name"] = QVariant::fromValue(QString("My-Seq"));

    m_treeView->setAsn1Value(asn1Value);

    QCOMPARE(m_treeView->getAsn1Value(), QString("{ field-a  FALSE, field-b  choice1 : FALSE }"));
}

QTEST_MAIN(tst_Asn1TreeView)

#include "tst_asn1treeview.moc"
