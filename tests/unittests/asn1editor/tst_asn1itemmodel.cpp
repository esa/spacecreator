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

#include "asn1editorconst.h"
#include "asn1itemmodel.h"
#include "asnsequencecomponent.h"
#include "sourcelocation.h"
#include "typeassignment.h"

#include "types/bitstring.h"
#include "types/boolean.h"
#include "types/choice.h"
#include "types/enumerated.h"
#include "types/ia5string.h"
#include "types/integer.h"
#include "types/null.h"
#include "types/numericstring.h"
#include "types/octetstring.h"
#include "types/real.h"
#include "types/sequence.h"
#include "types/sequenceof.h"

#include <QStandardItem>
#include <QtTest>
#include <memory>

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
    Asn1Acn::Types::Type::ASN1Type toAsn1Type(const QVariant &value)
    {
        return static_cast<Asn1Acn::Types::Type::ASN1Type>(value.toInt());
    }
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
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Integer>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyInt", "MyInt", location, std::move(type));
    itemModel->setAsn1Model(assignment);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyInt"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("INTEGER"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)), Asn1Acn::Types::Type::INTEGER);
}

void tst_Asn1ItemModel::testRealTypeModel()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Real>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyDouble", "MyDouble", location, std::move(type));
    itemModel->setAsn1Model(assignment);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyDouble"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("REAL"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)), Asn1Acn::Types::Type::REAL);
}

void tst_Asn1ItemModel::testIntTypeModelWithRange()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Integer>();
    type->setParameters({ { "min", 5 }, { "max", 15 } });
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyInt", "MyInt", location, std::move(type));
    itemModel->setAsn1Model(assignment);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyInt"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("INTEGER (5..15)"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)), Asn1Acn::Types::Type::INTEGER);
}

void tst_Asn1ItemModel::testRealTypeModelWithRange()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Real>();
    type->setParameters({ { "min", 10.0 }, { "max", 50.0 } });
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyDouble", "MyDouble", location, std::move(type));
    itemModel->setAsn1Model(assignment);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyDouble"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("REAL (10..50)"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)), Asn1Acn::Types::Type::REAL);
}

void tst_Asn1ItemModel::testBoolTypeModel()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Boolean>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyBool", "MyBool", location, std::move(type));
    itemModel->setAsn1Model(assignment);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyBool"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("BOOLEAN"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)), Asn1Acn::Types::Type::BOOLEAN);
}

void tst_Asn1ItemModel::testEnumTypeModel()
{
    QVariantList enumValues = { "enum1", "enum2", "enum3" };
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Enumerated>();
    type->setParameters({ { "values", enumValues } });
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyEnum", "MyEnum", location, std::move(type));
    itemModel->setAsn1Model(assignment);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyEnum"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("ENUMERATED"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)), Asn1Acn::Types::Type::ENUMERATED);
    QCOMPARE(itemModel->item(0, MODEL_VALUE_INDEX)->data(CHOICE_LIST_ROLE).toList().size(), 3);
}

void tst_Asn1ItemModel::testChoiceTypeModel()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Choice>();
    auto choice1 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            "choiceInt",
            "choiceInt",
            "choiceInt",
            "choiceInt",
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("choiceInt"));
    type->addComponent(std::move(choice1));
    auto choice2 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            "choiceReal",
            "choiceReal",
            "choiceReal",
            "choiceReal",
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Real>("choiceReal"));
    type->addComponent(std::move(choice2));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyChoice", "MyChoice", location, std::move(type));
    itemModel->setAsn1Model(assignment);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MyChoice"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("CHOICE"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)), Asn1Acn::Types::Type::CHOICE);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->rowCount(), 2);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0)->text(), QString("choiceInt"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0, MODEL_TYPE_INDEX)->text(), QString("INTEGER"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_NAME_INDEX)->child(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)),
            Asn1Acn::Types::Type::INTEGER);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1)->text(), QString("choiceReal"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1, MODEL_TYPE_INDEX)->text(), QString("REAL"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_NAME_INDEX)->child(1, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)),
            Asn1Acn::Types::Type::REAL);
}

void tst_Asn1ItemModel::testSequenceTypeModel()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Sequence>();
    auto sequence1 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "intVal",
            "intVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("intVal"));
    type->addComponent(std::move(sequence1));
    auto sequence2 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "realVal",
            "realVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Real>("realVal"));
    type->addComponent(std::move(sequence2));
    auto sequence3 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "boolVal",
            "boolVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Boolean>("boolVal"));
    type->addComponent(std::move(sequence3));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MySequence", "MySequence", location, std::move(type));
    itemModel->setAsn1Model(assignment);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->text(), QString("MySequence"));
    QCOMPARE(itemModel->item(0, MODEL_TYPE_INDEX)->text(), QString("SEQUENCE"));
    QVERIFY(itemModel->item(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).isNull() == true);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->rowCount(), 3);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0)->text(), QString("intVal"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(0, MODEL_TYPE_INDEX)->text(), QString("INTEGER"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_NAME_INDEX)->child(0, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)),
            Asn1Acn::Types::Type::INTEGER);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1)->text(), QString("realVal"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(1, MODEL_TYPE_INDEX)->text(), QString("REAL"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_NAME_INDEX)->child(1, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)),
            Asn1Acn::Types::Type::REAL);

    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(2)->text(), QString("boolVal"));
    QCOMPARE(itemModel->item(0, MODEL_NAME_INDEX)->child(2, MODEL_TYPE_INDEX)->text(), QString("BOOLEAN"));
    QCOMPARE(toAsn1Type(itemModel->item(0, MODEL_NAME_INDEX)->child(2, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE)),
            Asn1Acn::Types::Type::BOOLEAN);
}

QTEST_APPLESS_MAIN(tst_Asn1ItemModel)

#include "tst_asn1itemmodel.moc"
