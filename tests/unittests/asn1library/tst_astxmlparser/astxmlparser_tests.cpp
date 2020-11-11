/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "astxmlparser_tests.h"

#include "types/builtintypes.h"

#include <QtTest>

using namespace Asn1Acn::Tests;

AstXmlParserTests::AstXmlParserTests(QObject *parent)
    : QObject(parent)
{
}

void AstXmlParserTests::cleanup()
{
    m_parsedData.clear();
}

void AstXmlParserTests::setXmlData(const QString &str)
{
    m_xmlReader.clear();
    m_xmlReader.addData(str);
}

void AstXmlParserTests::test_emptyFile()
{
    parsingFails("");
}

void AstXmlParserTests::test_badXmlRoot()
{
    parsingFails(R"(<?xml version="1.0" encoding="utf-8"?>)"
                 R"(<BadXml></BadXml>)");
}

void AstXmlParserTests::test_emptyDefinitions()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="TestFile.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(  <AcnDependencies/>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["TestFile.asn"]->definitionsList().size(), std::size_t { 1 });
    const auto defs = m_parsedData["TestFile.asn"]->definitions("TestDefinitions");
    QCOMPARE(defs->name(), QStringLiteral("TestDefinitions"));
    QCOMPARE(defs->location().path(), QStringLiteral("TestFile.asn"));
    QCOMPARE(defs->location().line(), 13);
    QCOMPARE(defs->location().column(), 42);

    QCOMPARE(defs->parent(), m_parsedData["TestFile.asn"].get());
}

void AstXmlParserTests::test_singleTypeAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(            <Asn1Type>)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->types().size(), std::size_t { 1 });
    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt");
    QCOMPARE(type->name(), QStringLiteral("MyInt"));
    QCOMPARE(type->type()->identifier(), QStringLiteral("MyInt"));
    QCOMPARE(type->location().path(), QStringLiteral("Test2File.asn"));
    QCOMPARE(type->location().line(), 4);
    QCOMPARE(type->location().column(), 10);

    QCOMPARE(type->parent(), m_parsedData["Test2File.asn"]->definitions("TestDefinitions"));
}

void AstXmlParserTests::test_builtinTypeReference()
{
    QFETCH(QString, xmlTypeName);
    QFETCH(QString, typeName);
    QFETCH(Asn1Acn::Types::Type::ASN1Type, typeEnum);

    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(            <Asn1Type>)"
          R"(              <)"
            + xmlTypeName
            + "/>"
              R"(            </Asn1Type>)"
              R"(          </TypeAssignment>)"
              R"(        </TypeAssignments>)"
              R"(      </Module>)"
              R"(    </Modules>)"
              R"(  </Asn1File>)"
              R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->typeName(), typeName);
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->typeEnum(), typeEnum);
}

void AstXmlParserTests::test_builtinTypeReference_data()
{
    QTest::addColumn<QString>("xmlTypeName");
    QTest::addColumn<QString>("typeName");
    QTest::addColumn<Asn1Acn::Types::Type::ASN1Type>("typeEnum");

    // clang-format off
    QTest::newRow("Boolean")       << "BOOLEAN"       << "BOOLEAN"          << Asn1Acn::Types::Type::BOOLEAN;
    QTest::newRow("Null")          << "NULL"          << "NULL"             << Asn1Acn::Types::Type::NULLTYPE;
    QTest::newRow("Integer")       << "INTEGER"       << "INTEGER"          << Asn1Acn::Types::Type::INTEGER;
    QTest::newRow("Real")          << "REAL"          << "REAL"             << Asn1Acn::Types::Type::REAL;
    QTest::newRow("BitString")     << "BIT_STRING"    << "BIT STRING"       << Asn1Acn::Types::Type::BITSTRING;
    QTest::newRow("OctetString")   << "OCTET_STRING"  << "OCTET STRING"     << Asn1Acn::Types::Type::OCTETSTRING;
    QTest::newRow("IA5String")     << "IA5String"     << "IA5String"        << Asn1Acn::Types::Type::IA5STRING;
    QTest::newRow("NumericString") << "NumericString" << "NumericString"    << Asn1Acn::Types::Type::NUMERICSTRING;
    QTest::newRow("Enumerated")    << "ENUMERATED"    << "ENUMERATED"       << Asn1Acn::Types::Type::ENUMERATED;
    QTest::newRow("Choice")        << "CHOICE"        << "CHOICE"           << Asn1Acn::Types::Type::CHOICE;
    QTest::newRow("Sequence")      << "SEQUENCE"      << "SEQUENCE"         << Asn1Acn::Types::Type::SEQUENCE;
    QTest::newRow("SequenceOf")    << "SEQUENCE_OF"   << "SEQUENCE OF"      << Asn1Acn::Types::Type::SEQUENCEOF;
    // clang-format on
}

void AstXmlParserTests::test_userDefinedTypeReference()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <REFERENCE_TYPE Module="OtherModule" TypeAssignment="OrgInt"/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->typeName(), QStringLiteral("OrgInt"));

    const auto ref = m_parsedData["X.asn"]->referencesMap().find(3);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().column(), 19);
    QCOMPARE(ref->second->location().line(), 3);

    QCOMPARE(ref->second->name(), QStringLiteral("OrgInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("OtherModule"));
}

void AstXmlParserTests::test_multipleTypeAssignments()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <INTEGER/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(          <TypeAssignment Name="MyBool" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <BOOLEAN/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(          <TypeAssignment Name="MyNull" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <NULL/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->typeName(), QStringLiteral("INTEGER"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyBool")->type()->typeName(), QStringLiteral("BOOLEAN"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyNull")->type()->typeName(), QStringLiteral("NULL"));
}

void AstXmlParserTests::test_importedType()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedTypes>)"
          R"(                    <ImportedType Name="Imported" />)"
          R"(                </ImportedTypes>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(        <TypeAssignments/>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().size(), std::size_t(1));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).module(),
            QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).name(), QStringLiteral("Imported"));
}

void AstXmlParserTests::test_multipleImportedType()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedTypes>)"
          R"(                    <ImportedType Name="Imported1" />)"
          R"(                    <ImportedType Name="Imported2" />)"
          R"(                </ImportedTypes>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(        <TypeAssignments/>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().size(), std::size_t(2));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).name(), QStringLiteral("Imported1"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(1).name(), QStringLiteral("Imported2"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(1).module(),
            QStringLiteral("OtherDefinitions"));
}

void AstXmlParserTests::test_assignmentsAreTypeReferences()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeq" Line="19" CharPositionInLine="4">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <INTEGER/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2.asn"]->referencesMap().size(), std::size_t { 1 });

    const auto ref = m_parsedData["Test2.asn"]->referencesMap().find(19);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 19);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MySeq"));
    QCOMPARE(ref->second->module(), QStringLiteral("TestDefinitions"));
}

void AstXmlParserTests::test_sequenceTypeAssingment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeq" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <SEQUENCE>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <SEQUENCE_COMPONENT Name="a" Line="13" CharPositionInLine="3">)"
          R"(                  <Asn1Type Line="13" CharPositionInLine="8">)"
          R"(                    <INTEGER/>)"
          R"(                  </Asn1Type>)"
          R"(                </SEQUENCE_COMPONENT>)"
          R"(                <SEQUENCE_COMPONENT Name="b" Line="14" CharPositionInLine="2">)"
          R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
          R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                  </Asn1Type>)"
          R"(                </SEQUENCE_COMPONENT>)"
          R"(              </SEQUENCE>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeq");
    QCOMPARE(seq->type()->typeName(), QStringLiteral("SEQUENCE"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t { 2 });

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 14);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_choiceTypeAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyChoice" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <CHOICE>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <CHOICE_ALTERNATIVE Name="a" Line="13" CharPositionInLine="3">)"
          R"(                  <Asn1Type Line="13" CharPositionInLine="5">)"
          R"(                    <INTEGER/>)"
          R"(                  </Asn1Type>)"
          R"(                </CHOICE_ALTERNATIVE>)"
          R"(                <CHOICE_ALTERNATIVE Name="b" Line="14" CharPositionInLine="2">)"
          R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
          R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                  </Asn1Type>)"
          R"(                </CHOICE_ALTERNATIVE>)"
          R"(              </CHOICE>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto choice = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MyChoice");
    QCOMPARE(choice->type()->typeName(), QStringLiteral("CHOICE"));

    QCOMPARE(choice->location().line(), 11);
    QCOMPARE(choice->location().column(), 9);
    QCOMPARE(choice->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t { 2 });

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 14);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_sequenceOfTypeAssingment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeqOf" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <SEQUENCE_OF>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <Asn1Type Line="13" CharPositionInLine="4">)"
          R"(                  <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                </Asn1Type>)"
          R"(              </SEQUENCE_OF>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeqOf");
    QCOMPARE(seq->type()->typeName(), QStringLiteral("SEQUENCE OF"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t { 2 });

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(13);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 13);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_valueAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ValueAssignments>)"
          R"(          <ValueAssignment Name="asnConstant" Line="2" CharPositionInLine="0">)"
          R"(            <Asn1Type Line="2" CharPositionInLine="6">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </ValueAssignment>)"
          R"(        </ValueAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->values().size(), std::size_t { 1 });
    const auto var = m_parsedData["Test2File.asn"]->definitions("Defs")->value("asnConstant");
    QCOMPARE(var->name(), QStringLiteral("asnConstant"));
    QCOMPARE(var->location().line(), 2);
    QCOMPARE(var->type()->typeName(), QStringLiteral("INTEGER"));
}

void AstXmlParserTests::test_importedValues()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedValues>)"
          R"(                    <ImportedValue Name="Imported" />)"
          R"(                </ImportedValues>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().size(), std::size_t(1));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).module(),
            QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).name(),
            QStringLiteral("Imported"));
}

void AstXmlParserTests::test_multipleImportedValues()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedValues>)"
          R"(                    <ImportedValue Name="Imported" />)"
          R"(                    <ImportedValue Name="SecondImported" />)"
          R"(                </ImportedValues>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().size(), std::size_t(2));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).module(),
            QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).name(),
            QStringLiteral("Imported"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(1).name(),
            QStringLiteral("SecondImported"));
}

void AstXmlParserTests::test_multipleModules()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(      <Module Name="Defs2" Line="42" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt2" Line="99" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs")->type("MyInt")->location().line(), 11);
    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs2")->type("MyInt2")->location().line(), 99);
}

void AstXmlParserTests::test_multipleFiles()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(  <Asn1File FileName="File2.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs2" Line="4" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt2" Line="9" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs")->type("MyInt")->location().line(), 11);
    QCOMPARE(m_parsedData["File2.asn"]->definitions("Defs2")->type("MyInt2")->location().line(), 9);
}

void AstXmlParserTests::test_parametrizedInstancesContentsAreIgnored()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeq" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19" ParameterizedTypeInstance="true">)"
          R"(              <SEQUENCE>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <SEQUENCE_COMPONENT Name="b" Line="14" CharPositionInLine="2">)"
          R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
          R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                  </Asn1Type>)"
          R"(                </SEQUENCE_COMPONENT>)"
          R"(              </SEQUENCE>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeq");
    QCOMPARE(seq->type()->typeName(), QStringLiteral("SEQUENCE"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t { 1 });

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref == m_parsedData["Test2File.asn"]->referencesMap().end());
}

void AstXmlParserTests::test_asn1AstParsing()
{
    parse(R"(<?xml version="1.0" encoding="UTF-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="test.asn">)"
          R"(    <Asn1Module ID="ModuleTest">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MyInt" Line="6" CharPositionInLine="0">)"
          R"(          <Type Line="6" CharPositionInLine="10">)"
          R"(            <IntegerType Min="-5" Max="20" />)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(        <TypeAssignment Name="MyReal" Line="11" CharPositionInLine="3">)"
          R"(          <Type Line="11" CharPositionInLine="19">
          R"(            <RealType Min="0.00000000000000000000E+000" Max="1.00000000000000000000E+003" />)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(        <TypeAssignment Name="MyBOOL" Line="6" CharPositionInLine="0">)"
          R"(          <Type Line="6" CharPositionInLine="10">)"
          R"(            <BooleanType />)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(        <TypeAssignment Name="TypeEnumerated" Line="11" CharPositionInLine="3">)"
          R"(          <Type Line="11" CharPositionInLine="19">)"
          R"(            <EnumeratedType Extensible="False" ValuesAutoCalculated="False">)"
          R"(              <EnumValues>)"
          R"(                <EnumValue StringValue="red" IntValue="0" Line="17" CharPositionInLine="4" EnumID="red" />)"
          R"(                <EnumValue StringValue="green" IntValue="1" Line="18" CharPositionInLine="4" EnumID="green" />)"
          R"(                <EnumValue StringValue="blue" IntValue="2" Line="19" CharPositionInLine="4" EnumID="blue" />)"
          R"(              </EnumValues>)"
          R"(            </EnumeratedType>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(        <TypeAssignment Name="MyChoice" Line="9" CharPositionInLine="0">)"
          R"(          <Type Line="9" CharPositionInLine="13">)"
          R"(            <ChoiceType>)"
          R"(              <ChoiceChild VarName="hop" Line="9" CharPositionInLine="22" EnumID="hop_PRESENT">)"
          R"(                <Type Line="9" CharPositionInLine="26">)"
          R"(                  <BooleanType />)"
          R"(                </Type>)"
          R"(              </ChoiceChild>)"
          R"(              <ChoiceChild VarName="lat" Line="51" CharPositionInLine="4" EnumID="lat_PRESENT">)"
          R"(                <Type Line="51" CharPositionInLine="13">)"
          R"(                 <RealType Min="-9.00000000000000000000E+001" Max="9.00000000000000000000E+001" />)"
          R"(                </Type>)"
          R"(              </ChoiceChild>)"
          R"(            </ChoiceType>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(        <TypeAssignment Name="MySeq" Line="8" CharPositionInLine="0">)"
          R"(          <Type Line="8" CharPositionInLine="10">)"
          R"(            <SequenceType>)"
          R"(              <SequenceOrSetChild VarName="foo" Optional="False" Line="8" CharPositionInLine="21">)"
          R"(                <Type Line="8" CharPositionInLine="25">)"
          R"(                  <BooleanType />)"
          R"(                </Type>)"
          R"(              </SequenceOrSetChild>)"
          R"(              <SequenceOrSetChild VarName="int2Val" Optional="False" Line="26" CharPositionInLine="4">)"
          R"(                <Type Line="26" CharPositionInLine="12">)"
          R"(                  <IntegerType Min="-10" Max="10" />)"
          R"(                </Type>)"
          R"(              </SequenceOrSetChild>)"
          R"(            </SequenceType>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(        <TypeAssignment Name="T-UInt32" Line="3" CharPositionInLine="13">)"
          R"(          <Type Line="3" CharPositionInLine="13">)"
          R"(            <IntegerType Min="0" Max="4294967295"/>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QVERIFY(m_parsedData.find("test.asn") != m_parsedData.end());
    const Asn1Acn::Definitions *definitions = m_parsedData["test.asn"]->definitions("ModuleTest");
    QVERIFY(definitions != nullptr);

    const auto int1 = definitions->type("MyInt");
    QCOMPARE(int1->type()->typeName(), QStringLiteral("INTEGER"));
    auto intType1 = dynamic_cast<const Asn1Acn::Types::Integer *>(int1->type());
    QVERIFY(intType1->parameters().contains("min"));
    QCOMPARE(intType1->parameters()["min"].toLongLong(), -5);
    QVERIFY(intType1->parameters().contains("max"));
    QCOMPARE(intType1->parameters()["max"].toLongLong(), 20);

    const auto real1 = definitions->type("MyReal");
    QCOMPARE(real1->type()->typeName(), QStringLiteral("REAL"));
    auto realType1 = dynamic_cast<const Asn1Acn::Types::Real *>(real1->type());
    QVERIFY(realType1->parameters().contains("min"));
    QVERIFY(qFuzzyCompare(realType1->parameters()["min"].toDouble(), 0.0));
    QVERIFY(realType1->parameters().contains("max"));
    QVERIFY(qFuzzyCompare(realType1->parameters()["max"].toDouble(), 1000.0));

    const auto bool1 = definitions->type("MyBOOL");
    QCOMPARE(bool1->type()->typeName(), QStringLiteral("BOOLEAN"));

    const auto enum1 = definitions->type("TypeEnumerated");
    QCOMPARE(enum1->type()->typeName(), QStringLiteral("ENUMERATED"));
    auto enumType1 = dynamic_cast<const Asn1Acn::Types::Enumerated *>(enum1->type());
    QVERIFY(enumType1->parameters().contains("values"));
    QStringList values = enumType1->parameters()["values"].toStringList();
    QCOMPARE(values.size(), 3);
    QCOMPARE(values.at(0), QString("red"));
    QCOMPARE(values.at(1), QString("green"));
    QCOMPARE(values.at(2), QString("blue"));

    const auto choice1 = definitions->type("MyChoice");
    QCOMPARE(choice1->type()->typeName(), QStringLiteral("CHOICE"));
    auto choiceType1 = dynamic_cast<const Asn1Acn::Types::Choice *>(choice1->type());
    QCOMPARE(choiceType1->children().size(), 2);
    QCOMPARE(choiceType1->children().at(0)->typeName(), QStringLiteral("BOOLEAN"));
    QCOMPARE(choiceType1->children().at(0)->identifier(), QStringLiteral("hop"));
    QCOMPARE(choiceType1->children().at(1)->typeName(), QStringLiteral("REAL"));
    QCOMPARE(choiceType1->children().at(1)->identifier(), QStringLiteral("lat"));

    const auto sequence1 = definitions->type("MySeq");
    QCOMPARE(sequence1->type()->typeName(), QStringLiteral("SEQUENCE"));
    auto sequenceType1 = dynamic_cast<const Asn1Acn::Types::Sequence *>(sequence1->type());
    QCOMPARE(sequenceType1->children().size(), 2);
    QCOMPARE(sequenceType1->children().at(0)->typeName(), QStringLiteral("BOOLEAN"));
    QCOMPARE(sequenceType1->children().at(0)->identifier(), QStringLiteral("foo"));
    QCOMPARE(sequenceType1->children().at(1)->typeName(), QStringLiteral("INTEGER"));
    QCOMPARE(sequenceType1->children().at(1)->identifier(), QStringLiteral("int2Val"));

    const auto int2 = definitions->type("T-UInt32");
    QCOMPARE(int2->type()->typeName(), QStringLiteral("INTEGER"));
    auto intType2 = dynamic_cast<const Asn1Acn::Types::Integer *>(int2->type());
    QVERIFY(intType2->parameters().contains("min"));
    QCOMPARE(intType2->parameters()["min"].toLongLong(), 0);
    QVERIFY(intType2->parameters().contains("max"));
    QCOMPARE(intType2->parameters()["max"].toLongLong(), 4294967295);
}

void AstXmlParserTests::test_asn1AstReferenceParsing()
{
    parseTestFile("testmsc3.xml");
    QVERIFY(m_parsedData.find("testmsc3.asn") != m_parsedData.end());

    const Asn1Acn::Definitions *definitions = m_parsedData["testmsc3.asn"]->definitions("TETRIS-DATAVIEW");
    QVERIFY(definitions != nullptr);
    // Check is "Move-Status" is read and not skipped by "Sprite"
    const Asn1Acn::Definitions::Types &types = definitions->types();
    QCOMPARE(types.size(), 10);

    const Asn1Acn::Definitions *definitions2 = m_parsedData["testmsc3.asn"]->definitions("TASTE-BasicTypes");
    QVERIFY(definitions2 != nullptr);
    const Asn1Acn::Definitions::Types &types2 = definitions2->types();
    QCOMPARE(types2.size(), 5);
}

void AstXmlParserTests::parsingFails(const QString &xmlData)
{
    setXmlData(xmlData);
    AstXmlParser parser(m_xmlReader);
    QVERIFY(!parser.parse());
}

void AstXmlParserTests::parse(const QString &xmlData)
{
    setXmlData(xmlData);
    Asn1Acn::AstXmlParser parser(m_xmlReader);
    QVERIFY(parser.parse());
    m_parsedData = parser.takeData();
}

void AstXmlParserTests::parseTestFile(const QString &fileName)
{
    QFile testfile(QFINDTESTDATA(fileName));
    testfile.open(QIODevice::ReadOnly);
    m_xmlReader.clear();
    m_xmlReader.setDevice(&testfile);
    Asn1Acn::AstXmlParser parser(m_xmlReader);
    QVERIFY(parser.parse());
    m_parsedData = parser.takeData();
    m_xmlReader.clear();
}

QTEST_MAIN(AstXmlParserTests)
