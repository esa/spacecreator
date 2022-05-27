/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *`
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "tst_asn1topromelatranslator_values.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/multiplevalue.h>
#include <asn1library/asn1/namedvalue.h>
#include <asn1library/asn1/singlevalue.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/typefactory.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/valueassignment.h>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1nodevisitor.h>
#include <promela/PromelaModel/promelamodel.h>

using Asn1Acn::AsnSequenceComponent;
using Asn1Acn::IntegerValue;
using Asn1Acn::MultipleValue;
using Asn1Acn::NamedValue;
using Asn1Acn::SingleValue;
using Asn1Acn::SourceLocation;
using Asn1Acn::TypeAssignment;
using Asn1Acn::ValueAssignment;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Constraints::SizeConstraint;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using Asn1Acn::Types::Integer;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::SequenceOf;
using Asn1Acn::Types::TypeFactory;
using Asn1Acn::Types::UserdefinedType;
using promela::model::Assignment;
using promela::model::Constant;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::PromelaModel;
using promela::model::VariableRef;
using promela::translator::Asn1NodeVisitor;

namespace tmc::test {
void tst_Asn1ToPromelaTranslator_Values::initTestCase() {}

void tst_Asn1ToPromelaTranslator_Values::cleanupTestCase() {}

void tst_Asn1ToPromelaTranslator_Values::testSequence() const
{
    auto asn1Model = createModel();

    {
        auto mySeq = std::make_unique<Sequence>();

        auto component1 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field1"), QStringLiteral("field1"),
                false, std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
                TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));
        mySeq->addComponent(std::move(component1));
        auto component2 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field2"), QStringLiteral("field2"),
                true, std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
                TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));
        mySeq->addComponent(std::move(component2));
        auto component3 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field3"), QStringLiteral("field3"),
                true, std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
                TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));
        mySeq->addComponent(std::move(component3));

        auto typeAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MySeq"), QStringLiteral("MySeq"), SourceLocation(), mySeq->clone());
        asn1Model->addType(std::move(typeAssignment));

        auto seqReference = std::make_unique<UserdefinedType>("MySeq", "myModule");
        seqReference->setType(mySeq->clone());

        auto value = std::make_unique<NamedValue>();
        value->addValue("field1", std::make_unique<SingleValue>("3"));
        value->addValue("field2", std::make_unique<SingleValue>("4"));
        auto myValueAssignment = std::make_unique<ValueAssignment>(
                QStringLiteral("myValue"), SourceLocation(), std::move(seqReference), std::move(value));
        asn1Model->addValue(std::move(myValueAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getDeclarations().size(), 1);

    QCOMPARE(promelaModel.getDeclarations().at(0).getName(), "myValue");
    QVERIFY(promelaModel.getDeclarations().at(0).getType().isUtypeReference());
    QCOMPARE(promelaModel.getDeclarations().at(0).getType().getUtypeReference().getName(), "MySeq");

    QCOMPARE(promelaModel.getInlineDefs().size(), 5);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "myValue_init");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 4);
        {
            const InlineCall *call = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
            QVERIFY(call != 0);
            QCOMPARE(call->getName(), "MySeq_field1_assign_value");
            QCOMPARE(call->getArguments().size(), 2);
            QVERIFY(std::holds_alternative<VariableRef>(call->getArguments().at(0)));
            QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().size(), 2);
            QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().front().m_name, "myValue");
            QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().back().m_name, "field1");
            QVERIFY(std::holds_alternative<Constant>(call->getArguments().at(1)));
            QCOMPARE(std::get<Constant>(call->getArguments().at(1)).getValue(), 3);
        }
        {
            const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 1);
            QVERIFY(assignment != nullptr);
            QCOMPARE(assignment->getVariableRef().getElements().size(), 3);
            auto iter = assignment->getVariableRef().getElements().begin();
            QVERIFY(iter != assignment->getVariableRef().getElements().end());
            QCOMPARE(iter->m_name, "myValue");
            ++iter;
            QVERIFY(iter != assignment->getVariableRef().getElements().end());
            QCOMPARE(iter->m_name, "exist");
            ++iter;
            QVERIFY(iter != assignment->getVariableRef().getElements().end());
            QCOMPARE(iter->m_name, "field2");

            QVERIFY(std::holds_alternative<Constant>(assignment->getExpression().getContent()));
            QCOMPARE(std::get<Constant>(assignment->getExpression().getContent()).getValue(), 1);
        }
        {
            const InlineCall *call = findProctypeElement<InlineCall>(inlineDef->getSequence(), 2);
            QVERIFY(call != 0);
            QCOMPARE(call->getName(), "MySeq_field2_assign_value");
            QCOMPARE(call->getArguments().size(), 2);
            QVERIFY(std::holds_alternative<VariableRef>(call->getArguments().at(0)));
            QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().size(), 2);
            QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().front().m_name, "myValue");
            QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().back().m_name, "field2");
            QVERIFY(std::holds_alternative<Constant>(call->getArguments().at(1)));
            QCOMPARE(std::get<Constant>(call->getArguments().at(1)).getValue(), 4);
        }
        {
            const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 3);
            QVERIFY(assignment != nullptr);
            QCOMPARE(assignment->getVariableRef().getElements().size(), 3);
            auto iter = assignment->getVariableRef().getElements().begin();
            QVERIFY(iter != assignment->getVariableRef().getElements().end());
            QCOMPARE(iter->m_name, "myValue");
            ++iter;
            QVERIFY(iter != assignment->getVariableRef().getElements().end());
            QCOMPARE(iter->m_name, "exist");
            ++iter;
            QVERIFY(iter != assignment->getVariableRef().getElements().end());
            QCOMPARE(iter->m_name, "field3");

            QVERIFY(std::holds_alternative<Constant>(assignment->getExpression().getContent()));
            QCOMPARE(std::get<Constant>(assignment->getExpression().getContent()).getValue(), 0);
        }
    }
}

void tst_Asn1ToPromelaTranslator_Values::testInteger() const
{
    auto asn1Model = createModel();

    {
        auto integerType = std::make_unique<Integer>();
        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyInteger"), QStringLiteral("MyInteger"), SourceLocation(), integerType->clone());

        asn1Model->addType(std::move(myIntegerAssignment));

        auto integerReference = std::make_unique<UserdefinedType>("MyInteger", "myModule");
        integerReference->setType(integerType->clone());

        auto myValueAssignment = std::make_unique<ValueAssignment>(QStringLiteral("myValue"), SourceLocation(),
                std::move(integerReference), std::make_unique<SingleValue>("2"));
        asn1Model->addValue(std::move(myValueAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getDeclarations().size(), 1);

    QCOMPARE(promelaModel.getDeclarations().at(0).getName(), "myValue");
    QVERIFY(promelaModel.getDeclarations().at(0).getType().isUtypeReference());
    QCOMPARE(promelaModel.getDeclarations().at(0).getType().getUtypeReference().getName(), "MyInteger");

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "myValue_init");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);
        const InlineCall *call = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
        QVERIFY(call != 0);
        QCOMPARE(call->getName(), "MyInteger_assign_value");
        QCOMPARE(call->getArguments().size(), 2);
        QVERIFY(std::holds_alternative<VariableRef>(call->getArguments().at(0)));
        QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().size(), 1);
        QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().front().m_name, "myValue");
        QVERIFY(std::holds_alternative<Constant>(call->getArguments().at(1)));
        QCOMPARE(std::get<Constant>(call->getArguments().at(1)).getValue(), 2);
    }
}

void tst_Asn1ToPromelaTranslator_Values::testEnumerated() const
{
    auto asn1Model = createModel();

    {
        std::unique_ptr<Enumerated> enumType = std::make_unique<Enumerated>();
        enumType->addItem(EnumeratedItem(0, "zero", 0));
        enumType->addItem(EnumeratedItem(1, "one", 1));
        enumType->addItem(EnumeratedItem(2, "infinity", 100));

        auto myEnumAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyEnum"), QStringLiteral("MyEnum"), SourceLocation(), enumType->clone());

        asn1Model->addType(std::move(myEnumAssignment));

        auto enumReference = std::make_unique<UserdefinedType>("MyEnum", "myModule");
        enumReference->setType(enumType->clone());

        auto myValueAssignment = std::make_unique<ValueAssignment>(QStringLiteral("myValue"), SourceLocation(),
                std::move(enumReference), std::make_unique<SingleValue>("one"));
        asn1Model->addValue(std::move(myValueAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getDeclarations().size(), 1);

    QCOMPARE(promelaModel.getDeclarations().at(0).getName(), "myValue");
    QVERIFY(promelaModel.getDeclarations().at(0).getType().isUtypeReference());
    QCOMPARE(promelaModel.getDeclarations().at(0).getType().getUtypeReference().getName(), "MyEnum");

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "myValue_init");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);
        const InlineCall *call = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
        QVERIFY(call != 0);
        QCOMPARE(call->getName(), "MyEnum_assign_value");
        QCOMPARE(call->getArguments().size(), 2);
        QVERIFY(std::holds_alternative<VariableRef>(call->getArguments().at(0)));
        QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().size(), 1);
        QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().front().m_name, "myValue");
        QVERIFY(std::holds_alternative<VariableRef>(call->getArguments().at(1)));
        QCOMPARE(std::get<VariableRef>(call->getArguments().at(1)).getElements().size(), 1);
        QCOMPARE(std::get<VariableRef>(call->getArguments().at(1)).getElements().front().m_name, "MyEnum_one");
    }
}

void tst_Asn1ToPromelaTranslator_Values::testBoolean() const
{
    auto asn1Model = createModel();

    {
        auto booleanType = std::make_unique<Boolean>();
        auto myBooleanAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyBoolean"), QStringLiteral("MyBoolean"), SourceLocation(), booleanType->clone());

        asn1Model->addType(std::move(myBooleanAssignment));

        auto booleanReference = std::make_unique<UserdefinedType>("MyBoolean", "myModule");
        booleanReference->setType(booleanType->clone());

        auto myValueAssignment = std::make_unique<ValueAssignment>(QStringLiteral("myValue"), SourceLocation(),
                std::move(booleanReference), std::make_unique<SingleValue>("true"));
        asn1Model->addValue(std::move(myValueAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getDeclarations().size(), 1);

    QCOMPARE(promelaModel.getDeclarations().at(0).getName(), "myValue");
    QVERIFY(promelaModel.getDeclarations().at(0).getType().isUtypeReference());
    QCOMPARE(promelaModel.getDeclarations().at(0).getType().getUtypeReference().getName(), "MyBoolean");

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "myValue_init");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);
        const InlineCall *call = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
        QVERIFY(call != 0);
        QCOMPARE(call->getName(), "MyBoolean_assign_value");
        QCOMPARE(call->getArguments().size(), 2);
        QVERIFY(std::holds_alternative<VariableRef>(call->getArguments().at(0)));
        QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().size(), 1);
        QCOMPARE(std::get<VariableRef>(call->getArguments().at(0)).getElements().front().m_name, "myValue");
        QVERIFY(std::holds_alternative<Constant>(call->getArguments().at(1)));
        QCOMPARE(std::get<Constant>(call->getArguments().at(1)).getValue(), 1);
    }
}

void tst_Asn1ToPromelaTranslator_Values::testSequenceOf() const
{
    auto asn1Model = createModel();

    {
        auto integerType = std::make_unique<Integer>("MyInteger");
        auto myIntegerAssignment =
                std::make_unique<TypeAssignment>("MyInteger", "MyInteger", SourceLocation(), integerType->clone());

        asn1Model->addType(std::move(myIntegerAssignment));

        auto sequenceType = std::make_unique<SequenceOf>("MySequenceOf");
        sequenceType->setItemsType(integerType->clone());

        auto sizeConstraint =
                std::make_unique<SizeConstraint<IntegerValue>>(RangeConstraint<IntegerValue>::create({ 2, 5 }));
        sequenceType->constraints().append(std::move(sizeConstraint));

        auto mySequenceAssignment = std::make_unique<TypeAssignment>(
                "MySequenceOf", "MySequenceOf", SourceLocation(), sequenceType->clone());

        asn1Model->addType(std::move(mySequenceAssignment));

        auto sequenceReference = std::make_unique<UserdefinedType>("MySequenceOf", "myModule");
        sequenceReference->setType(sequenceType->clone());

        auto multipleValue = std::make_unique<MultipleValue>();
        multipleValue->addValue(std::make_unique<SingleValue>("1"));
        multipleValue->addValue(std::make_unique<SingleValue>("2"));
        multipleValue->addValue(std::make_unique<SingleValue>("3"));
        auto valueAssignment = std::make_unique<ValueAssignment>(
                "myValue", SourceLocation(), std::move(sequenceReference), std::move(multipleValue));
        asn1Model->addValue(std::move(valueAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    const auto &valueDeclarations = promelaModel.getDeclarations();
    QCOMPARE(valueDeclarations.size(), 1);

    const auto &valueDeclaration = valueDeclarations.at(0);
    QCOMPARE(valueDeclaration.getName(), "myValue");
    QVERIFY(valueDeclaration.getType().isUtypeReference());
    QCOMPARE(valueDeclaration.getType().getUtypeReference().getName(), "MySequenceOf");

    QCOMPARE(promelaModel.getInlineDefs().size(), 4);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "myValue_init");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 4);

        const auto &sequence = inlineDef->getSequence();

        const auto assignIndex0 = findProctypeElement<InlineCall>(sequence, 0);
        QVERIFY(assignIndex0);
        QCOMPARE(assignIndex0->getName(), "MyInteger_assign_value");
        QCOMPARE(assignIndex0->getArguments().size(), 2);

        const auto &assignIndex0Args = assignIndex0->getArguments();

        const auto assignIndex0Arg0 = std::get_if<VariableRef>(&assignIndex0Args.at(0));
        QVERIFY(assignIndex0Arg0);
        const auto &assignIndex0Arg0Elements = assignIndex0Arg0->getElements();
        QCOMPARE(assignIndex0Arg0Elements.size(), 2);
        QCOMPARE(assignIndex0Arg0Elements.front().m_name, "myValue");
        QCOMPARE(assignIndex0Arg0Elements.back().m_name, "data");
        const auto &assignIndex0Arg0Index =
                std::get_if<Constant>(&assignIndex0Arg0Elements.back().m_index->getContent());
        QVERIFY(assignIndex0Arg0Index);
        QCOMPARE(assignIndex0Arg0Index->getValue(), 0);

        const auto assignIndex0Arg1 = std::get_if<Constant>(&assignIndex0Args.at(1));
        QVERIFY(assignIndex0Arg1);
        QCOMPARE(assignIndex0Arg1->getValue(), 1);

        const auto assignIndex1 = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(assignIndex1);
        QCOMPARE(assignIndex1->getName(), "MyInteger_assign_value");
        QCOMPARE(assignIndex1->getArguments().size(), 2);

        const auto &assignIndex1Args = assignIndex1->getArguments();

        const auto assignIndex1Arg0 = std::get_if<VariableRef>(&assignIndex1Args.at(0));
        QVERIFY(assignIndex1Arg0);
        const auto &assignIndex1Arg0Elements = assignIndex1Arg0->getElements();
        QCOMPARE(assignIndex1Arg0Elements.size(), 2);
        QCOMPARE(assignIndex1Arg0Elements.front().m_name, "myValue");
        QCOMPARE(assignIndex1Arg0Elements.back().m_name, "data");
        const auto &assignIndex1Arg0Index =
                std::get_if<Constant>(&assignIndex1Arg0Elements.back().m_index->getContent());
        QVERIFY(assignIndex1Arg0Index);
        QCOMPARE(assignIndex1Arg0Index->getValue(), 1);

        const auto assignIndex1Arg1 = std::get_if<Constant>(&assignIndex1Args.at(1));
        QVERIFY(assignIndex1Arg1);
        QCOMPARE(assignIndex1Arg1->getValue(), 2);

        const auto assignIndex2 = findProctypeElement<InlineCall>(sequence, 2);
        QVERIFY(assignIndex2);
        QCOMPARE(assignIndex2->getName(), "MyInteger_assign_value");
        QCOMPARE(assignIndex2->getArguments().size(), 2);

        const auto &assignIndex2Args = assignIndex2->getArguments();

        const auto assignIndex2Arg0 = std::get_if<VariableRef>(&assignIndex2Args.at(0));
        QVERIFY(assignIndex2Arg0);
        const auto &assignIndex2Arg0Elements = assignIndex2Arg0->getElements();
        QCOMPARE(assignIndex2Arg0Elements.size(), 2);
        QCOMPARE(assignIndex2Arg0Elements.front().m_name, "myValue");
        QCOMPARE(assignIndex2Arg0Elements.back().m_name, "data");
        const auto &assignIndex2Arg0Index =
                std::get_if<Constant>(&assignIndex2Arg0Elements.back().m_index->getContent());
        QVERIFY(assignIndex2Arg0Index);
        QCOMPARE(assignIndex2Arg0Index->getValue(), 2);

        const auto assignIndex2Arg1 = std::get_if<Constant>(&assignIndex2Args.at(1));
        QVERIFY(assignIndex2Arg1);
        QCOMPARE(assignIndex2Arg1->getValue(), 3);

        const auto assignLength = findProctypeElement<Assignment>(sequence, 3);
        QVERIFY(assignLength);

        const auto assignLengthVar = assignLength->getVariableRef();
        const auto &assignLengthVarElements = assignLengthVar.getElements();
        QCOMPARE(assignLengthVarElements.size(), 2);
        QCOMPARE(assignLengthVarElements.front().m_name, "myValue");
        QCOMPARE(assignLengthVarElements.back().m_name, "length");

        const auto assignLengthConst = std::get_if<Constant>(&assignLength->getExpression().getContent());
        QVERIFY(assignLengthConst);
        QCOMPARE(assignLengthConst->getValue(), 3);
    }
}

std::unique_ptr<Definitions> tst_Asn1ToPromelaTranslator_Values::createModel() const
{
    return std::make_unique<Definitions>("myModule", SourceLocation());
}

const InlineDef *tst_Asn1ToPromelaTranslator_Values::findInline(
        const std::list<std::unique_ptr<InlineDef>> &list, const QString &name) const
{
    auto iter = std::find_if(list.begin(), list.end(),
            [&](const std::unique_ptr<InlineDef> &proctype) { return proctype->getName() == name; });

    if (iter == list.end()) {
        return nullptr;
    }
    return iter->get();
}

template<typename T>
const T *tst_Asn1ToPromelaTranslator_Values::findProctypeElement(
        const ::promela::model::Sequence &sequence, size_t index) const
{
    auto iter = sequence.getContent().begin();
    std::advance(iter, index);
    if (iter == sequence.getContent().end()) {
        return nullptr;
    }
    if (std::holds_alternative<T>((*iter)->getValue())) {
        return &(std::get<T>((*iter)->getValue()));
    }
    return nullptr;
}

}
