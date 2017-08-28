/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.pl/Space
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

#include "parseddocumentbuilder_tests.h"

#include <QtTest>

#include <QSignalSpy>

#include "../parseddocument.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

ParsedDocumentBuilderTests::ParsedDocumentBuilderTests(QObject *parent)
    : QObject(parent),
      m_serviceProvider(new Asn1SccServiceProviderPhony)
{
}

ParsedDocumentBuilderTests::~ParsedDocumentBuilderTests()
{
    delete m_serviceProvider;
}

void ParsedDocumentBuilderTests::test_failed()
{
    DocumentSourceInfo sourceInfo;
    sourceInfo.setName("FAILED");

    QHash<QString, DocumentSourceInfo> documents;
    documents.insert("FAILED", sourceInfo);

    ParsedDocumentBuilder *builder = new ParsedDocumentBuilder(documents, m_serviceProvider);
    QSignalSpy spyFailed(builder, &ParsedDocumentBuilder::failed);
    QSignalSpy spyErrored(builder, &ParsedDocumentBuilder::errored);
    QSignalSpy spyFinished(builder, &ParsedDocumentBuilder::finished);

    builder->run();

    QTest::qWait(200);

    QCOMPARE(spyFailed.count(), 1);
    QCOMPARE(spyErrored.count(), 0);
    QCOMPARE(spyFinished.count(), 0);

    std::vector<std::unique_ptr<ParsedDocument>> parsedDocuments = builder->takeDocuments();
    QVERIFY(parsedDocuments.size() == 0);

    delete builder;
}

void ParsedDocumentBuilderTests::test_error()
{
    DocumentSourceInfo sourceInfo;
    sourceInfo.setName("ERROR");
    sourceInfo.setContent("{\"ErrorCode\":2,\"Files\":null,\"Messages\":[\"Asn1.asn:8:13: error: No type assignment with name 'Number4' found in the module 'OtherEmptyAsn1'\"]}");

    QHash<QString, DocumentSourceInfo> documents;
    documents.insert("ERROR", sourceInfo);

    ParsedDocumentBuilder *builder = new ParsedDocumentBuilder(documents, m_serviceProvider);
    QSignalSpy spyFailed(builder, &ParsedDocumentBuilder::failed);
    QSignalSpy spyErrored(builder, &ParsedDocumentBuilder::errored);
    QSignalSpy spyFinished(builder, &ParsedDocumentBuilder::finished);

    builder->run();

    QTest::qWait(200);

    QCOMPARE(spyFailed.count(), 0);
    QCOMPARE(spyErrored.count(), 1);
    QCOMPARE(spyFinished.count(), 0);

    QStringList errorMessages = builder->errorMessages();
    QCOMPARE(errorMessages.count(), 1);
    QCOMPARE(errorMessages.at(0), QString("Asn1.asn:8:13: error: No type assignment with name 'Number4' found in the module 'OtherEmptyAsn1'"));

    std::vector<std::unique_ptr<ParsedDocument>> parsedDocuments = builder->takeDocuments();
    QVERIFY(parsedDocuments.size() == 0);

    delete builder;
}

void ParsedDocumentBuilderTests::test_success()
{
    DocumentSourceInfo sourceInfo;
    sourceInfo.setName("SUCCESS");
    sourceInfo.setContent("{\"ErrorCode\":0,\"Files\":[{\"Contents\":\"<?xml version=\\\"1.0\\\" encoding=\\\"utf-8\\\"?>\\u000a<ASN1AST>\\u000a    "
                          "<Asn1File FileName=\\\"emptyAsn2.asn\\\">\\u000a        <Asn1Module ID=\\\"EmptyAsn2\\\">\\u000a        "
                          "<ExportedTypes>\\u000a            <ExportedType Name=\\\"AData\\\" \\/>\\u000a            "
                          "<ExportedType Name=\\\"BData\\\" \\/> \\u000a        <\\/ExportedTypes>\\u000a        <ExportedVariables>\\u000a         "
                          "\\u000a        <\\/ExportedVariables>\\u000a        <ImportedModules>\\u000a        <\\/ImportedModules>\\u000a        "
                          "<TypeAssignments>\\u000a            <TypeAssignment Name=\\\"AData\\\" Line=\\\"2\\\" CharPositionInLine=\\\"4\\\">\\u000a                "
                          "<Type Line=\\\"2\\\" CharPositionInLine=\\\"14\\\">\\u000a                    "
                          "<IntegerType Min=\\\"-100\\\" Max=\\\"1000\\\"\\/>\\u000a                <\\/Type>\\u000a            "
                          "<\\/TypeAssignment>\\u000a            <TypeAssignment Name=\\\"BData\\\" Line=\\\"3\\\" CharPositionInLine=\\\"4\\\">\\u000a                "
                          "<Type Line=\\\"3\\\" CharPositionInLine=\\\"14\\\">\\u000a                    <IntegerType Min=\\\"0\\\" Max=\\\"1000\\\"\\/>\\u000a                "
                          "<\\/Type>\\u000a            <\\/TypeAssignment>\\u000a        <\\/TypeAssignments>\\u000a        <VariablesAssignments>\\u000a        "
                          "<\\/VariablesAssignments>\\u000a        <\\/Asn1Module>\\u000a    <\\/Asn1File>\\u000a<\\/ASN1AST>\",\"Name\":\"AST.xml\"}],\"Messages\":null}");

    QHash<QString, DocumentSourceInfo> documents;
    documents.insert("SUCCESS", sourceInfo);

    ParsedDocumentBuilder *builder = new ParsedDocumentBuilder(documents, m_serviceProvider);
    QSignalSpy spyFailed(builder, &ParsedDocumentBuilder::failed);
    QSignalSpy spyErrored(builder, &ParsedDocumentBuilder::errored);
    QSignalSpy spyFinished(builder, &ParsedDocumentBuilder::finished);

    builder->run();

    QTest::qWait(200);

    QCOMPARE(spyFailed.count(), 0);
    QCOMPARE(spyErrored.count(), 0);
    QCOMPARE(spyFinished.count(), 1);

    std::vector<std::unique_ptr<ParsedDocument>> parsedDocuments = builder->takeDocuments();
    QCOMPARE(parsedDocuments.size(), static_cast<size_t>(1));

    delete builder;
}
