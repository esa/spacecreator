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

#include "asn1const.h"
#include "asn1itemmodel.h"

#include <QStandardItem>
#include <QtTest>

using namespace asn1;

class tst_Asn1ItemModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void testModel();

    void testIntTypeModel();
    void testRealTypeModel();
    void testIntTypeModelWithRange();
    void testRealTypeModelWithRange();
    void testBoolTypeModel();
    void testEnumTypeModel();
    void testChoiceTypeModel();
    void testSequenceTypeModel();

private:
    Asn1ItemModel *itemModel = nullptr;
};

void tst_Asn1ItemModel::init()
{
    itemModel = new Asn1ItemModel();
}

void tst_Asn1ItemModel::cleanup()
{
    delete itemModel;
    itemModel = nullptr;
}

void tst_Asn1ItemModel::testModel()
{
    QCOMPARE(itemModel->rowCount(), 1);
    QCOMPARE(itemModel->columnCount(), 4);
}

void tst_Asn1ItemModel::testIntTypeModel()
{
    itemModel->setAsn1Model({ { "name", "MyInt" }, { "type", INTEGER } });

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyInt"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("integer"));
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), INTEGER);
}

void tst_Asn1ItemModel::testRealTypeModel()
{
    itemModel->setAsn1Model({ { "name", "MyDouble" }, { "type", DOUBLE } });

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyDouble"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("double"));
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), DOUBLE);
}

void tst_Asn1ItemModel::testIntTypeModelWithRange()
{
    itemModel->setAsn1Model({ { "name", "MyInt" }, { "type", INTEGER }, { "min", 5 }, { "max", 15 } });

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyInt"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("integer (5..15)"));
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), INTEGER);
}

void tst_Asn1ItemModel::testRealTypeModelWithRange()
{
    itemModel->setAsn1Model({ { "name", "MyDouble" }, { "type", DOUBLE }, { "min", 10.0 }, { "max", 50.0 } });

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyDouble"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("double (10..50)"));
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), DOUBLE);
}

void tst_Asn1ItemModel::testBoolTypeModel()
{
    itemModel->setAsn1Model({ { "name", "MyBool" }, { "type", BOOL } });

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyBool"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("bool"));
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), BOOL);
}

void tst_Asn1ItemModel::testEnumTypeModel()
{
    QVariantList enumValues = { "enum1", "enum2", "enum3" };
    itemModel->setAsn1Model({ { "name", "MyEnum" }, { "type", ENUMERATED }, { "values", enumValues } });

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyEnum"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("enumerated"));
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), ENUMERATED);
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(CHOICE_LIST_ROLE).toList().size(), 3);
}

void tst_Asn1ItemModel::testChoiceTypeModel()
{
    QVariantList choices;
    QVariantMap choice = { { "name", "choiceInt" }, { "type", INTEGER } };
    choices.append(choice);

    choice = { { "name", "choiceReal" }, { "type", DOUBLE } };
    choices.append(choice);

    itemModel->setAsn1Model({ { "name", "MyChoice" }, { "type", CHOICE }, { "choices", choices } });

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyChoice"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("choice"));
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), CHOICE);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->rowCount(), 2);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0)->text(), QString("choiceInt"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0, MODEL_TYPE_INDEX)->text(), QString("integer"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), INTEGER);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1)->text(), QString("choiceReal"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1, MODEL_TYPE_INDEX)->text(), QString("double"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), DOUBLE);
}

void tst_Asn1ItemModel::testSequenceTypeModel()
{
    QVariantList children;
    QVariantMap child = { { "name", "intVal" }, { "type", INTEGER } };
    children.append(child);

    child = { { "name", "realVal" }, { "type", DOUBLE } };
    children.append(child);

    child = { { "name", "boolVal" }, { "type", BOOL } };
    children.append(child);

    itemModel->setAsn1Model({ { "name", "MySequence" }, { "type", SEQUENCE }, { "children", children } });

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MySequence"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("sequence"));
    QVERIFY(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).isNull() == true);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->rowCount(), 3);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0)->text(), QString("intVal"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0, MODEL_TYPE_INDEX)->text(), QString("integer"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), INTEGER);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1)->text(), QString("realVal"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1, MODEL_TYPE_INDEX)->text(), QString("double"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), DOUBLE);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(2)->text(), QString("boolVal"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(2, MODEL_TYPE_INDEX)->text(), QString("bool"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(2, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt(), BOOL);
}

QTEST_APPLESS_MAIN(tst_Asn1ItemModel)

#include "tst_asn1itemmodel.moc"
