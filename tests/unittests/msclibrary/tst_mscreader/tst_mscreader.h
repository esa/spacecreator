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

#pragma once

#include "mscreader.h"

#include <QtTest>

class tst_MscReader : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testFileOpenError();
    void testSyntaxError();
    void testExampleFilesParsing_data();
    void testExampleFilesParsing();
    void testEmptyDocument();
    void testComments();
    void testEntityComments();
    void testNestedDocuments();
    void testMscInDocument();
    void testInstance();
    void testInstanceWithKind();
    void testGateMessage();
    void testCondition();
    void testCoregion();
    void testTimer();
    void testTimerRelation();
    void testAction();
    void testInstanceStop_data();
    void testInstanceStop();
    void testHierarchy();
    void testDataDefinitionLanguage();
    void testDataDefinitionData();
    void testKeywordAsName();
    void testNonStandardInstance();
    void testDocumentsType_data();
    void testDocumentsType();
    void testDefaultDocumentTypeFromLoad();
    void testMessageDeclaration();
    void testNameFiltering();

    // Instance create tests - in tst_msccreateparsing
    void testInstanceCreate();
    void testInstanceCreateNoParameter();
    void testInstanceCreateMultiParameter();
    void testInstanceCreateEmptyParameter();
    void testInstanceCreateNoInstance();
    void testInstanceCreateDublicate();
    void testMessageCreateInstance();

    // Message related tests - in tst_mscmessageparsing.cpp
    void testMessage();
    void testSameMessageInTwoInstances();
    void testMessageWithParameters();
    void testMessageParameterWildcard();
    void testMessageParameterExpression();
    void testMultiParameters();
    void testMessageParametersCurlyBraces();
    void testMessageChoiceParameter();
    void testMessageComplexParameter();
    void testMessageAsn1SequenceChoiceParameter();
    void testMessageAsn1SequenceOfSecencesParameter();
    void testMessageAsn1OctetString();
    void testMessageAsn1SequenceOfInSequence();
    void testSortedMessage();
    void testSortedMessageTwoCharts();
    void testSortedInstanceEvents();
    void testSortedMessageCreate();
    void testSortingDeadlock();
    void testIncompleteMessageIn();
    void testIncompleteMessageOut();
    void testConditionDublicate();
    void testTestMessageInstanceName();
    void testSameNameDifferentInstances();
    void testDifferentParameter();
    void testMultiMessageOccurrence();
    void testNonStandardVia();

private:
    msc::MscReader *m_reader = nullptr;
};
