/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Fuzzer - Tool for generating test cases
** based on ASN.1/ACN models and simulating malformed or malicious data.
**
** Tool was developed under a programme and funded by
** European Space Agency.
**
** This Tool is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Tool is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "tst_astxmlconstraintparser.h"

#include "constraints/printingvisitor.h"

#include <QtTest>

using namespace Asn1Acn::Tests;
using namespace Asn1Acn;

AstXmlConstraintParserTests::AstXmlConstraintParserTests(QObject *parent)
    : QObject(parent)
{
}

void AstXmlConstraintParserTests::test_emptyXml()
{
    parse<IntegerValue>("");

    QVERIFY(m_dumpedConstraint.isEmpty());
}

void AstXmlConstraintParserTests::test_simpleIntegerValue()
{
    parse<IntegerValue>("<IntegerValue>1410</IntegerValue>");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("(1410)"));
}

void AstXmlConstraintParserTests::test_simpleRealValue()
{
    parse<RealValue>("<RealValue>2.5</RealValue>");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("(2.5)"));
}

void AstXmlConstraintParserTests::test_simpleBooleanValue()
{
    parse<BooleanValue>("<BooleanValue>true</BooleanValue>");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("(TRUE)"));
}

void AstXmlConstraintParserTests::test_simpleEnumValue()
{
    parse<EnumValue>("<EnumValue>x</EnumValue>");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("(x)"));
}

void AstXmlConstraintParserTests::test_simpleStringValue()
{
    parse<StringValue>("<StringValue>abc</StringValue>");

    QCOMPARE(m_dumpedConstraint, QStringLiteral(R"(("abc"))"));
}

void AstXmlConstraintParserTests::test_simpleBitStringValue()
{
    parse<BitStringValue>("<BitStringValue>0101</BitStringValue>");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("('0101'B)"));
}

void AstXmlConstraintParserTests::test_simpleOctetStringValue()
{
    parse<OctetStringValue>("<OctetStringValue>10</OctetStringValue>");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("('10'H)"));
}

void AstXmlConstraintParserTests::test_range()
{
    parse<IntegerValue>(R"(
                              <Range>
                                <Min>
                                  <IntegerValue>12</IntegerValue>
                                </Min>
                                <Max>
                                  <IntegerValue>34</IntegerValue>
                                </Max>
                              </Range>
                              )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("(12 .. 34)"));
}

void AstXmlConstraintParserTests::test_rangeWithoutMinOrMax()
{
    parsingFails<IntegerValue>(R"(
                              <Range>
                                <Min>
                                  <IntegerValue>12</IntegerValue>
                                </Min>
                              </Range>
                              )");

    parsingFails<IntegerValue>(R"(
                              <Range>
                                <Min>
                                </Min>
                                <Max>
                                  <IntegerValue>34</IntegerValue>
                                </Max>
                              </Range>
                              )");
}

void AstXmlConstraintParserTests::test_multipleValues()
{
    parse<IntegerValue>(R"(<Constraints>
                                    <IntegerValue>12</IntegerValue>
                                    <IntegerValue>22</IntegerValue>
                                 </Constraints>)");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("(12) (22)"));
}

void AstXmlConstraintParserTests::test_or()
{
    parse<EnumValue>(R"(
                           <OR>
                                <EnumValue>a</EnumValue>
                                <EnumValue>b</EnumValue>
                           </OR>
                           )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("((a | b))"));
}

void AstXmlConstraintParserTests::test_and()
{
    parse<EnumValue>(R"(
                           <AND>
                                <EnumValue>a</EnumValue>
                                <EnumValue>b</EnumValue>
                           </AND>
                           )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("((a ^ b))"));
}

void AstXmlConstraintParserTests::test_nestedLogicalOperators()
{
    parse<EnumValue>(R"(
                           <AND>
                                <EnumValue>a</EnumValue>
                                <OR>
                                    <EnumValue>b</EnumValue>
                                    <EnumValue>c</EnumValue>
                                </OR>
                           </AND>
                           )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("((a ^ (b | c)))"));
}

void AstXmlConstraintParserTests::test_from()
{
    parse<StringValue>(R"(
                             <ALPHA>
                                <Range>
                                    <Min><StringValue>A</StringValue></Min>
                                    <Max><StringValue>Z</StringValue></Max>
                                </Range>
                             </ALPHA>
                           )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral(R"((FROM("A" .. "Z")))"));
}

void AstXmlConstraintParserTests::test_size()
{
    parse<StringValue>(R"(
                             <SIZE>
                                <Range>
                                    <Min><IntegerValue>1</IntegerValue></Min>
                                    <Max><IntegerValue>5</IntegerValue></Max>
                                </Range>
                             </SIZE>
                           )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral("(SIZE(1 .. 5))"));
}

void AstXmlConstraintParserTests::test_mixed()
{
    parse<StringValue>(R"(<Constraints>
                             <ALPHA>
                                <Range>
                                    <Min><StringValue>A</StringValue></Min>
                                    <Max><StringValue>Z</StringValue></Max>
                                </Range>
                             </ALPHA>
                             <SIZE>
                                <Range>
                                    <Min><IntegerValue>1</IntegerValue></Min>
                                    <Max><IntegerValue>5</IntegerValue></Max>
                                </Range>
                             </SIZE>
                             </Constraints>
                           )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral(R"((FROM("A" .. "Z")) (SIZE(1 .. 5)))"));
}

void AstXmlConstraintParserTests::test_mixedInReverseOrder()
{
    parse<StringValue>(R"(<Constraints>
                             <SIZE>
                                <Range>
                                    <Min><IntegerValue>1</IntegerValue></Min>
                                    <Max><IntegerValue>5</IntegerValue></Max>
                                </Range>
                             </SIZE>
                             <ALPHA>
                                <Range>
                                    <Min><StringValue>A</StringValue></Min>
                                    <Max><StringValue>Z</StringValue></Max>
                                </Range>
                             </ALPHA>
                             </Constraints>
                           )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral(R"((SIZE(1 .. 5)) (FROM("A" .. "Z")))"));
}

void AstXmlConstraintParserTests::test_complex()
{
    parse<StringValue>(R"(
                             <OR>
                                <AND>
                                    <SIZE>
                                        <Range>
                                            <Min><IntegerValue>1</IntegerValue></Min>
                                            <Max><IntegerValue>5</IntegerValue></Max>
                                        </Range>
                                    </SIZE>
                                    <ALPHA>
                                        <Range>
                                            <Min><StringValue>A</StringValue></Min>
                                            <Max><StringValue>Z</StringValue></Max>
                                        </Range>
                                    </ALPHA>
                                </AND>
                                <StringValue>ZXY</StringValue>
                             </OR>
                           )");

    QCOMPARE(m_dumpedConstraint, QStringLiteral(R"((((SIZE(1 .. 5) ^ FROM("A" .. "Z")) | "ZXY")))"));
}

template<typename T>
void AstXmlConstraintParserTests::parse(const QString &xmlData)
{
    QXmlStreamReader xmlReader(xmlData);
    Constraints::ConstraintList<T> list;
    AstXmlConstraintParser<T> parser(xmlReader, list);
    QVERIFY(parser.parse());
    m_dumpedConstraint = toString(list);
}

template<typename T>
void AstXmlConstraintParserTests::parsingFails(const QString &xmlData)
{
    QXmlStreamReader xmlReader(xmlData);
    Constraints::ConstraintList<T> list;
    AstXmlConstraintParser<T> parser(xmlReader, list);
    QVERIFY(!parser.parse());
}

QTEST_MAIN(AstXmlConstraintParserTests)
