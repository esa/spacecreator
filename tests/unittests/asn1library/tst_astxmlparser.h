/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
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

#pragma once

#include "astxmlparser.h"

#include <QObject>
#include <QXmlStreamReader>

namespace Asn1Acn {
namespace Tests {

class AstXmlParserTests : public QObject
{
    Q_OBJECT
public:
    explicit AstXmlParserTests(QObject *parent = 0);

private Q_SLOTS:
    void cleanup();
    void test_emptyFile();
    void test_badXmlRoot();
    void test_emptyDefinitions();
    void test_singleTypeAssignment();
    void test_builtinTypeReference();
    void test_builtinTypeReference_data();
    void test_userDefinedTypeReference();
    void test_multipleTypeAssignments();
    void test_importedType();
    void test_multipleImportedType();
    void test_assignmentsAreTypeReferences();
    void test_sequenceTypeAssingment();
    void test_sequenceOfTypeAssingmentOfReferencedType();
    void test_sequenceOfTypeAssingmentOfBuiltinType();
    void test_choiceTypeAssignment();
    void test_valueAssignment();
    void test_importedValues();
    void test_multipleImportedValues();
    void test_multipleModules();
    void test_multipleFiles();
    void test_parametrizedInstancesContentsAreIgnored();
    void test_asn1AstParsing();
    void test_asn1AstReferenceParsing();

    void test_enumeratedItems();
    void test_enumeratedConstraints();
    void test_enumeratedWithAcnParams();
    void test_enumeratedValueAssignment();

    void test_singleIntegerTypeAssignmentWithSimpleConstraint();
    void test_singleIntegerTypeAssignmentWithRangedConstraints();
    void test_singleIntegerTypeAssignmentWithAcnParams();
    void test_singleIntegerValueAssignment();

    void test_singleRealTypeAssignmentWithSimpleConstraint();
    void test_singleRealTypeAssignmentWithRangedConstraints();
    void test_singleRealTypeAssignmentWithAcnParams();
    void test_singleRealValueAssignment();

    void test_sequenceOfAssignmentWithSimpleConstraint();
    void test_sequenceOfAssignmentWithMultipleRangeConstraints();
    void test_sequenceOfAssignmentWithRangeConstraintInsideSizeConstraint();
    void test_sequenceOfAssignmentWithAcnParams();
    void test_sequenceOfValueAssignment();

    void test_choiceWithAcnParams();
    void test_choiceAlternatives();
    void test_choiceAlternativesWithAcnParams();
    void test_choiceValueAssignment();

    void test_booleanWithAcnParams();
    void test_booleanValueAssignment();

    void test_nullWithAcnParams();
    void test_nullValueAssignment();

    void test_sequnceWithAcnParams();
    void test_sequenceComponents();
    void test_sequenceComponentsWithAcnParams();
    void test_sequenceAcnComponents();
    void test_sequenceValueAssignment();
    void test_sequenceDefaultValue();

    void test_octetStringWithSizeConstraint();
    void test_octetStringWithRangedSizeConstraint();
    void test_octetStringWithValueDefined();
    void test_octetStringAcnParams();
    void test_octetStringValueAssignment();

    void test_iA5StringWithSizeConstraint();
    void test_iA5StringWithValueConstraint();
    void test_iA5StringWithValueDefined();
    void test_iA5StringAcnParams();
    void test_iA5StringValueAssignment();

    void test_numericStringWithSizeConstraint();
    void test_numericStringWithValueConstraint();
    void test_numericStringWithValueDefined();
    void test_numericStringAcnParams();

    void test_bitStringWithSizeConstraint();
    void test_bitStringWithRangedSizeConstraint();
    void test_bitStringWithValueDefined();
    void test_bitStringAcnParams();
    void test_bitStringValueAssignment();

    void test_notRelatedConstraintsInNumericString();
    void test_nestedValueAssignments();
    void test_parametrizedInstances();

private:
    template<typename Collection>
    auto itemFromCollection(const Collection &col, const QString &id);

    void setXmlData(const QString &str);
    void parsingFails(const QString &xmlData);
    void parse(const QString &xmlData);
    void parseTestFile(const QString &fileName);

    QXmlStreamReader m_xmlReader;
    std::map<QString, std::unique_ptr<Asn1Acn::File>> m_parsedData;
};

}
}
