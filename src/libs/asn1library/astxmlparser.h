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
#pragma once

#include "asn1/acnargument.h"
#include "asn1/acnparameter.h"
#include "asn1/acnsequencecomponent.h"
#include "asn1/definitions.h"
#include "asn1/file.h"
#include "asn1/namedvalue.h"
#include "asn1/types/type.h"
#include "asn1/value.h"

#include <QString>
#include <QXmlStreamReader>
#include <memory>

namespace Asn1Acn {

class AstXmlParser
{
public:
    explicit AstXmlParser(QXmlStreamReader &xmlReader);

    bool parse();

    std::map<QString, std::unique_ptr<File>> takeData() { return std::move(m_data); }

private:
    void readAstRoot();
    void readAsn1File();
    void readModules();
    void readModule();
    void readExportedTypes();
    void readExportedValues();
    void readImportedModules();
    void readTypeAssignments();
    void readValueAssignments();
    void readModuleChildren();
    void readTypeAssignment();
    void readValueAssignment();

    void updateCurrentFile();
    void createNewModule();
    QString readIdAttribute(const QString &id);
    QString readNameAttribute();
    QString readCNameAttribute();
    QString readVarNameAttribute();
    QString readTypeAttribute();
    int readLineAttribute();
    int readCharPossitionInLineAttribute();
    bool isParametrizedTypeInstance() const;

    void readImportedModule();
    void readImportedValues(const QString &moduleName);
    void readImportedValue(const QString &moduleName);
    void readImportedTypes(const QString &moduleName);
    void readImportedType(const QString &moduleName);

    SourceLocation readLocationFromAttributes();
    QString readIsAlignedToNext();

    ValuePtr findAndReadValueAssignmentValue();
    ValuePtr readValueAssignmentValue();

    ValuePtr readSimpleValue(const QStringRef &name);
    ValuePtr readMultipleValue();
    ValuePtr readSequenceValues();
    std::pair<QString, ValuePtr> readNamedValue();
    ValuePtr readChoiceValue();

    std::unique_ptr<Types::Type> findAndReadType();
    std::unique_ptr<Types::Type> readType();
    std::unique_ptr<Types::Type> readTypeDetails(const QString &name, const SourceLocation &location,
            const bool isParametrized, const QString &typeAlignment);
    std::unique_ptr<Types::Type> buildTypeFromName(
            const QString &name, const SourceLocation &location, bool isParametrized);

    std::unique_ptr<Types::Type> createReferenceType(const SourceLocation &location);
    void readReferredTypeDetails(Types::Type &type);
    void readAcnArguments(Types::Type &type);

    AcnParameterPtrs readAcnParameters();
    AcnParameterPtr readAcnParameter();

    void readTypeContents(const QString &name, Types::Type &type);
    void readTypeAttributes(Types::Type &type);

    void readSequence(Types::Type &type);
    void readSequenceComponent(Types::Type &type);
    void readAcnComponent(Types::Type &type);

    void readSequenceOf(Types::Type &type);

    void readChoice(Types::Type &type);

    void readReferenceType(Types::Type &type);

    void readInteger(Types::Type &type);
    void readReal(Types::Type &type);

    void readEnumerated(Types::Type &type);
    void readEnumeratedItem(Types::Type &type);

    void readOctetString(Types::Type &type);
    void readIA5String(Types::Type &type);
    void readNumericString(Types::Type &type);
    void readBitString(Types::Type &type);

    void findAndReadConstraints(Types::Type &type);
    void readConstraints(Types::Type &type);

    QString readTypeAssignmentAttribute();
    QString readModuleAttribute();

    bool nextRequiredElementIs(const QString &name);
    bool nextElementIs(const QString &name);
    bool skipToChildElement(const QString &name);

    QXmlStreamReader &m_xmlReader;
    std::map<QString, std::unique_ptr<File>> m_data;
    QString m_currentFile;
    QString m_currentModule;

    Definitions *m_currentDefinitions;
    bool m_inParametrizedBranch;
};

}
