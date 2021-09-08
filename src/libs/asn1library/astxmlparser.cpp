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

#include <iostream>

#include "asn1const.h"
#include "astxmlparser.h"

#include "asn1/asnsequencecomponent.h"
#include "asn1/choicevalue.h"
#include "asn1/multiplevalue.h"
#include "asn1/namedvalue.h"
#include "asn1/singlevalue.h"
#include "asn1/sourcelocation.h"

#include "asn1/types/acnparameterizablecomposite.h"
#include "asn1/types/bitstring.h"
#include "asn1/types/boolean.h"
#include "asn1/types/choice.h"
#include "asn1/types/enumerated.h"
#include "asn1/types/ia5string.h"
#include "asn1/types/integer.h"
#include "asn1/types/integeracnparams.h"
#include "asn1/types/null.h"
#include "asn1/types/numericstring.h"
#include "asn1/types/octetstring.h"
#include "asn1/types/real.h"
#include "asn1/types/sequence.h"
#include "asn1/types/sequenceof.h"
#include "asn1/types/typefactory.h"
#include "asn1/types/typemutatingvisitor.h"
#include "asn1/types/typevisitorwithvalue.h"
#include "asn1/types/userdefinedtype.h"

#include "astxmlconstraintparser.h"

using namespace Asn1Acn;

static std::function<QString(QString)> getPrinterFunction(const QString &name)
{
    if (name == QStringLiteral("StringValue"))
        return StringValue::asString;
    if (name == QStringLiteral("OctetStringValue"))
        return OctetStringValue::asString;
    if (name == QStringLiteral("BitStringValue"))
        return BitStringValue::asString;
    if (name == QStringLiteral("BooleanValue"))
        return BooleanValue::reformatString;

    return {};
}

namespace {
class TypeAttributesAssigningVisitor : public Types::TypeMutatingVisitor
{
public:
    TypeAttributesAssigningVisitor(const QXmlStreamAttributes &attributes)
        : m_attributes(attributes)
    {}

    ~TypeAttributesAssigningVisitor() override {}

    void visit(Types::Boolean &type) override
    {
        type.setTrueValue(m_attributes.value(QLatin1String("true-value")).toString());
        type.setFalseValue(m_attributes.value(QLatin1String("false-value")).toString());
    }

    void visit(Types::Null &type) override
    {
        type.setPattern(m_attributes.value(QLatin1String("pattern")).toString());
    }

    void visit(Types::BitString &type) override
    {
        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
    }

    void visit(Types::OctetString &type) override
    {
        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
    }

    void visit(Types::IA5String &type) override
    {
        using namespace Types;

        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
        type.setTerminationPattern(
            m_attributes.value(QStringLiteral("termination-pattern")).toString());
        type.setEncoding(IA5String::mapEncoding(m_attributes.value(QStringLiteral("encoding"))));
    }

    void visit(Types::NumericString &type) override
    {
        using namespace Types;

        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
        type.setTerminationPattern(
            m_attributes.value(QStringLiteral("termination-pattern")).toString());
        type.setEncoding(NumericString::mapEncoding(m_attributes.value(QStringLiteral("encoding"))));
    }

    void visit(Types::Enumerated &type) override
    {
        setIntegerAcnParameters(type);

        const auto encodeValues = m_attributes.value(QStringLiteral("encode-values"));
        type.setEncodeValues(encodeValues.toString().toLower() == "true");
    }

    void visit(Types::Choice &type) override
    {
        type.setDeterminant(m_attributes.value(QLatin1String("determinant")).toString());
    }

    void visit(Types::Sequence &type) override { Q_UNUSED(type); }

    void visit(Types::SequenceOf &type) override
    {
        type.setAcnSize(m_attributes.value(QLatin1String("size")).toString());
    }

    void visit(Types::Real &type) override
    {
        using namespace Types;

        type.setEndianness(Type::mapEndianess(m_attributes.value(QLatin1String("endianness"))));
        type.setEncoding(Real::mapEncoding(m_attributes.value(QLatin1String("encoding"))));
    }

    void visit(Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Types::Integer &type) override { setIntegerAcnParameters(type); }

    void visit(Types::UserdefinedType &type) override { Q_UNUSED(type); }

private:
    void setIntegerAcnParameters(Types::IntegerAcnParameters &type)
    {
        using namespace Types;

        type.setSize(m_attributes.value(QLatin1String("size")).toInt());
        type.setEndianness(Type::mapEndianess(m_attributes.value(QLatin1String("endianness"))));
        type.setEncoding(Integer::mapEncoding(m_attributes.value(QLatin1String("encoding"))));
        type.setAcnMinSizeInBits(m_attributes.value(QLatin1String("acnMinSizeInBits")).toInt());
        type.setAcnMaxSizeInBits(m_attributes.value(QLatin1String("acnMaxSizeInBits")).toInt());
    }

    const QXmlStreamAttributes &m_attributes;
};

class ChildItemAddingVisitor : public Types::TypeMutatingVisitor
{
public:
    ChildItemAddingVisitor(const QXmlStreamAttributes &attributes,
                           const QString &currentFile,
                           std::unique_ptr<Types::Type> childType = nullptr)
        : m_attributes(attributes)
        , m_currentFile(currentFile)
        , m_childType(std::move(childType))
    {}

    ~ChildItemAddingVisitor() override {}

    void visit(Types::Boolean &type) override { Q_UNUSED(type); }
    void visit(Types::Null &type) override { Q_UNUSED(type); }
    void visit(Types::BitString &type) override { Q_UNUSED(type); }
    void visit(Types::OctetString &type) override { Q_UNUSED(type); }
    void visit(Types::IA5String &type) override { Q_UNUSED(type); }
    void visit(Types::NumericString &type) override { Q_UNUSED(type); }
    void visit(Types::Enumerated &type) override { Q_UNUSED(type); }

    void visit(Types::Choice &type) override
    {
        const auto name = readStringAttribute(QStringLiteral("Name"));
        const auto presentWhenName = readStringAttribute(QStringLiteral("PresentWhenName"));
        const auto adaNameAttribute = readStringAttribute(QStringLiteral("AdaName"));
        const auto cNameAttribute = readStringAttribute(QStringLiteral("CName"));
        const auto presentWhen = readStringAttribute(QStringLiteral("present-when"));

        SourceLocation location(m_currentFile,
                                      readIntegerAttribute(QStringLiteral("Line")),
                                      readIntegerAttribute(QStringLiteral("CharPositionInLine")));

        type.addComponent(std::make_unique<Types::ChoiceAlternative>(name,
                                                                           presentWhenName,
                                                                           adaNameAttribute,
                                                                           cNameAttribute,
                                                                           presentWhen,
                                                                           location,
                                                                           std::move(m_childType)));
    }

    void visit(Types::Sequence &type) override
    {
        const auto name = readStringAttribute(QStringLiteral("Name"));
        const auto cName = readStringAttribute(QStringLiteral("CName"));
        const auto presentWhen = readStringAttribute(QStringLiteral("present-when"));
        const auto optional = readStringAttribute(QStringLiteral("Optional")).toLower();
        SourceLocation location(m_currentFile,
                                      readIntegerAttribute(QStringLiteral("Line")),
                                      readIntegerAttribute(QStringLiteral("CharPositionInLine")));

        type.addComponent(std::make_unique<AsnSequenceComponent>(name,
                                                                       cName,
                                                                       optional == "true",
                                                                       presentWhen,
                                                                       location,
                                                                       std::move(m_childType)));
    }

    void visit(Types::SequenceOf &type) override
    {
        type.setItemsType(std::move(m_childType));
    }

    void visit(Types::Real &type) override { Q_UNUSED(type); }
    void visit(Types::LabelType &type) override { Q_UNUSED(type); }
    void visit(Types::Integer &type) override { Q_UNUSED(type); }
    void visit(Types::UserdefinedType &type) override
    {
        type.setType(std::move(m_childType));
    }

protected:
    int readIntegerAttribute(const QString &key) const { return m_attributes.value(key).toInt(); }
    QString readStringAttribute(const QString &key) const
    {
        return m_attributes.value(key).toString();
    }
    const QString &currentFile() const { return m_currentFile; }

private:
    const QXmlStreamAttributes &m_attributes;
    const QString &m_currentFile;
    std::unique_ptr<Types::Type> m_childType;
};

class EnumeratedItemAddingVisitor : public ChildItemAddingVisitor
{
public:
    EnumeratedItemAddingVisitor(const QXmlStreamAttributes &attributes,
                                const QString &currentFile,
                                int index)
        : ChildItemAddingVisitor(attributes, currentFile)
        , m_index(index)
    {}

    void visit(Types::Enumerated &type) override
    {
        const auto itemName = readStringAttribute(QStringLiteral("Name"));
        const auto value = readIntegerAttribute(QStringLiteral("Value"));
        SourceLocation location(currentFile(),
                                      readIntegerAttribute(QStringLiteral("Line")),
                                      readIntegerAttribute(QStringLiteral("CharPositionInLine")));
        type.addItem({m_index, itemName, value, location});
    }

private:
    const int m_index;
};

class AcnDefinedItemsAddingVisitor : public Types::TypeMutatingVisitor
{
public:
    AcnDefinedItemsAddingVisitor(AcnParameterPtrs acnParameters)
        : m_acnParameters(std::move(acnParameters))
    {}

    AcnDefinedItemsAddingVisitor(AcnArgumentPtrs acnArguments)
        : m_acnArguments(std::move(acnArguments))
    {}

    AcnDefinedItemsAddingVisitor(AcnSequenceComponentPtr acnComponent)
        : m_acnComponent(std::move(acnComponent))
    {}

    ~AcnDefinedItemsAddingVisitor() override {}

    void visit(Types::Boolean &type) override { Q_UNUSED(type); }
    void visit(Types::Null &type) override { Q_UNUSED(type); }
    void visit(Types::BitString &type) override { Q_UNUSED(type); }
    void visit(Types::OctetString &type) override { Q_UNUSED(type); }
    void visit(Types::IA5String &type) override { Q_UNUSED(type); }
    void visit(Types::NumericString &type) override { Q_UNUSED(type); }
    void visit(Types::Enumerated &type) override { Q_UNUSED(type); }
    void visit(Types::Choice &type) override
    {
        for (auto &param : m_acnParameters)
            type.addParameter(std::move(param));
    }

    void visit(Types::Sequence &type) override
    {
        for (auto &param : m_acnParameters)
            type.addParameter(std::move(param));

        if (m_acnComponent)
            type.addComponent(std::move(m_acnComponent));
    }

    void visit(Types::SequenceOf &type) override { Q_UNUSED(type); }
    void visit(Types::Real &type) override { Q_UNUSED(type); }
    void visit(Types::LabelType &type) override { Q_UNUSED(type); }
    void visit(Types::Integer &type) override { Q_UNUSED(type); }
    void visit(Types::UserdefinedType &type) override
    {
        for (auto &arg : m_acnArguments)
            type.addArgument(std::move(arg));
    }

private:
    AcnParameterPtrs m_acnParameters;
    AcnArgumentPtrs m_acnArguments;
    AcnSequenceComponentPtr m_acnComponent;
};
} // namespace

AstXmlParser::AstXmlParser(QXmlStreamReader &xmlReader)
    : m_xmlReader(xmlReader)
    , m_currentDefinitions(nullptr)
{}

bool AstXmlParser::parse()
{
    if (nextRequiredElementIs({{QStringLiteral("AstRoot")}, {QStringLiteral("ASN1AST")}}))
        readAstRoot();

    const bool ok = !m_xmlReader.hasError();
    if (!ok) {
        std::cerr << m_xmlReader.errorString().toStdString() << std::endl;
    }
    return ok;
}

void AstXmlParser::readAstRoot()
{
    while (skipToChildElement(QStringLiteral("Asn1File")))
        readAsn1File();
}

void AstXmlParser::readAsn1File()
{
    updateCurrentFile();

    if (!m_xmlReader.readNextStartElement()) {
        m_xmlReader.skipCurrentElement();
        return;
    }


    if (m_xmlReader.name() == QStringLiteral("Modules")) {
        readModules();
        m_xmlReader.skipCurrentElement();
        return;
    }

    if (m_xmlReader.name() == QStringLiteral("Asn1Module")) {
        readModule();
        readModules();
        if (m_xmlReader.name().toString() == QStringLiteral("Asn1Module")) {
            m_xmlReader.skipCurrentElement();
        }
        return;
    }

    m_xmlReader.raiseError(QString("XML does not contain expected <%1> element")
                                   .arg(QStringLiteral("Modules"), QStringLiteral("Asn1Module")));
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readModules()
{
    while (nextElementIs({{QStringLiteral("Module")}, {QStringLiteral("Asn1Module")}}))
        readModule();
}

void AstXmlParser::updateCurrentFile()
{
    m_currentFile = m_xmlReader.attributes().value(QStringLiteral("FileName")).toString();
    m_data.insert(std::make_pair(m_currentFile, std::make_unique<File>(m_currentFile)));
}

void AstXmlParser::readModuleChildren()
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("ExportedTypes"))
            readExportedTypes();
        else if (m_xmlReader.name() == QStringLiteral("ExportedValues"))
            readExportedValues();
        else if (m_xmlReader.name() == QStringLiteral("ImportedModules"))
            readImportedModules();
        else if (m_xmlReader.name() == QStringLiteral("TypeAssignments"))
            readTypeAssignments();
        else if (m_xmlReader.name() == QStringLiteral("ValueAssignments"))
            readValueAssignments();
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::createNewModule()
{
    const auto location = readLocationFromAttributes();
    m_currentModule = readNameAttribute();
    if (m_currentModule.isEmpty()) {
        m_currentModule = readIdAttribute("ID");
    }
    auto module = std::make_unique<Definitions>(m_currentModule, location);
    m_currentDefinitions = module.get();
    m_data[m_currentFile]->add(std::move(module));
}

void AstXmlParser::readModule()
{
    createNewModule();
    readModuleChildren();
}

QString AstXmlParser::readIdAttribute(const QString &id)
{
    return m_xmlReader.attributes().value(id).toString();
}

void AstXmlParser::readValueAssignments()
{
    while (nextElementIs(QStringLiteral("ValueAssignment")))
        readValueAssignment();
}

void AstXmlParser::readTypeAssignments()
{
    while (nextElementIs(QStringLiteral("TypeAssignment")))
        readTypeAssignment();
}

void AstXmlParser::readTypeAssignment()
{
    Q_ASSERT(m_currentDefinitions != nullptr);

    m_inParametrizedBranch = false;

    const auto location = readLocationFromAttributes();
    const auto name = readNameAttribute();
    const auto cname = readCNameAttribute();
    auto type = findAndReadType();
    type->setIdentifier(name);
    m_xmlReader.skipCurrentElement();

    m_currentDefinitions->addType(
        std::make_unique<TypeAssignment>(name, cname, location, std::move(type)));

    m_data[m_currentFile]->addTypeReference(
        std::make_unique<TypeReference>(name, m_currentDefinitions->name(), location));
}

void AstXmlParser::readValueAssignment()
{
    Q_ASSERT(m_currentDefinitions != nullptr);

    const auto location = readLocationFromAttributes();
    const auto name = readNameAttribute();

    auto type = findAndReadType();
    auto value = findAndReadValueAssignmentValue();

    m_currentDefinitions->addValue(
        std::make_unique<ValueAssignment>(name, location, std::move(type), std::move(value)));
}

ValuePtr AstXmlParser::findAndReadValueAssignmentValue()
{
    if (!m_xmlReader.readNextStartElement())
        return nullptr;

    auto val = readValueAssignmentValue();
    m_xmlReader.skipCurrentElement();

    return val;
}

static bool isSingleValueName(const QStringRef &name)
{
    // clang-format off
    return name == QStringLiteral("IntegerValue")
           || name == QStringLiteral("RealValue")
           || name == QStringLiteral("StringValue")
           || name == QStringLiteral("EnumValue")
           || name == QStringLiteral("OctetStringValue")
           || name == QStringLiteral("BitStringValue")
           || name == QStringLiteral("BooleanValue");
    // clang-format on
}

static bool isMultipleValueName(const QStringRef &name)
{
    return name == QStringLiteral("SequenceOfValue");
}

static bool isSequenceValue(const QStringRef &name)
{
    return name == QStringLiteral("SequenceValue");
}

static bool isChoiceValue(const QStringRef &name)
{
    return name == QStringLiteral("ChoiceValue");
}

static bool isNullValue(const QStringRef &name)
{
    return name == QStringLiteral("NullValue");
}

ValuePtr AstXmlParser::readValueAssignmentValue()
{
    ValuePtr value = nullptr;

    const auto name = m_xmlReader.name();

    if (isSingleValueName(name))
        value = readSimpleValue(name);
    else if (isMultipleValueName(name))
        value = readMultipleValue();
    else if (isSequenceValue(name))
        value = readSequenceValues();
    else if (isChoiceValue(name)) {
        value = readChoiceValue();
        m_xmlReader.skipCurrentElement();
    } else if (isNullValue(name)) {
        value = std::make_unique<SingleValue>(QStringLiteral("NULL"));
        m_xmlReader.skipCurrentElement();
    } else
        m_xmlReader.skipCurrentElement();

    return value;
}

ValuePtr AstXmlParser::readSimpleValue(const QStringRef &name)
{
    return std::make_unique<SingleValue>(m_xmlReader.readElementText(),
                                               getPrinterFunction(name.toString()));
}

ValuePtr AstXmlParser::readMultipleValue()
{
    auto value = std::make_unique<MultipleValue>();

    while (m_xmlReader.readNextStartElement())
        value->addValue(readValueAssignmentValue());

    return std::move(value);
}

ValuePtr AstXmlParser::readSequenceValues()
{
    auto value = std::make_unique<NamedValue>();
    while (skipToChildElement(QStringLiteral("NamedValue"))) {
        auto v = readNamedValue();
        value->addValue(v.first, std::move(v.second));
    }

    return std::move(value);
}

ValuePtr AstXmlParser::readChoiceValue()
{
    if (!skipToChildElement(QStringLiteral("NamedValue")))
        return nullptr;

    auto v = readNamedValue();

    return std::make_unique<ChoiceValue>(v.first, std::move(v.second));
}

std::pair<QString, ValuePtr> AstXmlParser::readNamedValue()
{
    auto name = readNameAttribute();
    if (!m_xmlReader.readNextStartElement())
        return {};

    auto value = readValueAssignmentValue();
    m_xmlReader.skipCurrentElement();

    return std::make_pair(name, std::move(value));
}

QString AstXmlParser::readTypeAssignmentAttribute()
{
    if (m_xmlReader.attributes().hasAttribute(QStringLiteral("ReferencedTypeName"))) {
        return m_xmlReader.attributes().value(QStringLiteral("ReferencedTypeName")).toString();
    } else {
        return m_xmlReader.attributes().value(QStringLiteral("TypeAssignment")).toString();
    }
}

QString AstXmlParser::readModuleAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Module")).toString();
}

QString AstXmlParser::readNameAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Name")).toString();
}

QString AstXmlParser::readCNameAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("CName")).toString();
}

QString AstXmlParser::readVarNameAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("VarName")).toString();
}

QString AstXmlParser::readTypeAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Type")).toString();
}

int AstXmlParser::readLineAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Line")).toInt();
}

int AstXmlParser::readCharPossitionInLineAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("CharPositionInLine")).toInt();
}

void AstXmlParser::readImportedModules()
{
    while (nextRequiredElementIs(QStringLiteral("ImportedModule")))
        readImportedModule();
}

void AstXmlParser::readImportedModule()
{
    const auto moduleName = readIdAttribute(QStringLiteral("ID"));
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("ImportedTypes"))
            readImportedTypes(moduleName);
        else if (m_xmlReader.name() == QStringLiteral("ImportedValues"))
            readImportedValues(moduleName);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readImportedValues(const QString &moduleName)
{
    while (nextRequiredElementIs(QStringLiteral("ImportedValue")))
        readImportedValue(moduleName);
}

void AstXmlParser::readImportedValue(const QString &moduleName)
{
    m_currentDefinitions->addImportedValue({moduleName, readNameAttribute()});
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readImportedTypes(const QString &moduleName)
{
    while (nextRequiredElementIs(QStringLiteral("ImportedType")))
        readImportedType(moduleName);
}

void AstXmlParser::readImportedType(const QString &moduleName)
{
    m_currentDefinitions->addImportedType({moduleName, readNameAttribute()});
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readExportedValues()
{
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readExportedTypes()
{
    m_xmlReader.skipCurrentElement();
}

bool AstXmlParser::nextRequiredElementIs(const QString &name)
{
    if (!m_xmlReader.readNextStartElement())
        return false;
    if (m_xmlReader.name() == name)
        return true;
    m_xmlReader.raiseError(QString("XML does not contain expected <%1> element").arg(name));
    return false;
}

bool AstXmlParser::nextRequiredElementIs(const QStringList &names)
{
    const bool ok = nextElementIs(names);
    if (!ok) {
        m_xmlReader.raiseError(QString("XML does not contain expected <%1> element").arg(names.join(",")));
    }
    return ok;
}

bool AstXmlParser::nextElementIs(const QString &name)
{
    if (!m_xmlReader.readNextStartElement()) {
        return false;
    }
    return m_xmlReader.name() == name;
}

bool AstXmlParser::nextElementIs(const QStringList &names)
{
    if (!m_xmlReader.readNextStartElement()) {
        return false;
    }
    return names.contains(m_xmlReader.name().toString());
}

SourceLocation AstXmlParser::readLocationFromAttributes()
{
    return {m_currentFile, readLineAttribute(), readCharPossitionInLineAttribute()};
}

QString AstXmlParser::readIsAlignedToNext()
{
    return m_xmlReader.attributes().value(QLatin1String("align-to-next")).toString();
}

std::unique_ptr<Types::Type> AstXmlParser::findAndReadType()
{
    if (!skipToChildElement({{QStringLiteral("Asn1Type")}, {QStringLiteral("Type")}}))
        return {};
    return readType();
}

static bool isTypeName(const QString &name)
{
    // clang-format off
    return name == QStringLiteral("BIT_STRING")
           || name == QStringLiteral("BIT STRING")
           || name.startsWith(QStringLiteral("BitString"), Qt::CaseInsensitive)
           || name.startsWith(QStringLiteral("Boolean"), Qt::CaseInsensitive)
           || name.startsWith(QStringLiteral("Choice"), Qt::CaseInsensitive)
           || name.startsWith(QStringLiteral("Enumerated"), Qt::CaseInsensitive)
           || name.startsWith(QStringLiteral("IA5String"), Qt::CaseInsensitive)
           || name.startsWith(QStringLiteral("Integer"), Qt::CaseInsensitive)
           || name == QStringLiteral("NULL")
           || name == QStringLiteral("NullType")
           || name.startsWith(QStringLiteral("NumericString"), Qt::CaseInsensitive)
           || name == QStringLiteral("OCTET_STRING")
           || name == QStringLiteral("OCTET STRING")
           || name == QStringLiteral("OctetStringType")
           || name == QStringLiteral("REAL")
           || name.startsWith(QStringLiteral("Real"), Qt::CaseInsensitive)
           || name == QStringLiteral("SEQUENCE")
           || name == QStringLiteral("SequenceType")
           || name == QStringLiteral("SEQUENCE_OF")
           || name == QStringLiteral("SEQUENCE OF")
           || name == QStringLiteral("SequenceOfType")
           || name == QStringLiteral("REFERENCE_TYPE")
           || name == QStringLiteral("REFERENCE TYPE")
           || name == QStringLiteral("ReferenceType");
    // clang-format on
}

std::unique_ptr<Types::Type> AstXmlParser::readType()
{
    const auto location = readLocationFromAttributes();
    const auto isParametrized = isParametrizedTypeInstance();
    const auto alignToNext = readIsAlignedToNext();
    std::unique_ptr<Types::Type> type = nullptr;
    AcnParameterPtrs acnParameters;

    while (m_xmlReader.readNextStartElement()) {
        const auto name = m_xmlReader.name().toString();

        if (name == QStringLiteral("AcnParameters"))
            acnParameters = readAcnParameters();
        else if (isTypeName(name))
            type = readTypeDetails(name, location, isParametrized, alignToNext);
        else
            m_xmlReader.skipCurrentElement();
    }

    AcnDefinedItemsAddingVisitor visitor(std::move(acnParameters));
    type->accept(visitor);

    return type;
}

AcnParameterPtrs AstXmlParser::readAcnParameters()
{
    AcnParameterPtrs parameters;

    while (skipToChildElement(QStringLiteral("AcnParameter")))
        parameters.push_back(readAcnParameter());

    return parameters;
}

AcnParameterPtr AstXmlParser::readAcnParameter()
{
    const auto id = readIdAttribute(QStringLiteral("Id"));
    const auto name = readNameAttribute();
    const auto type = readTypeAttribute();

    m_xmlReader.skipCurrentElement();

    return std::make_unique<AcnParameter>(id, name, type);
}

bool AstXmlParser::isParametrizedTypeInstance() const
{
    const auto value = m_xmlReader.attributes().value(QStringLiteral("ParameterizedTypeInstance"));
    return !value.isNull() && value == QStringLiteral("true");
}

std::unique_ptr<Types::Type> AstXmlParser::readTypeDetails(const QString &name,
                                                                 const SourceLocation &location,
                                                                 const bool isParametrized,
                                                                 const QString &typeAlignment)
{
    auto type = buildTypeFromName(name, location, isParametrized);
    type->setAlignToNext(Types::Type::mapAlignToNext(typeAlignment));

    return type;
}

std::unique_ptr<Types::Type> AstXmlParser::buildTypeFromName(
    const QString &name, const SourceLocation &location, bool isParametrized)
{
    auto type = (name == QStringLiteral("REFERENCE_TYPE") || name == QStringLiteral("ReferenceType"))
                    ? createReferenceType(location)
                    : Types::TypeFactory::createBuiltinType(name);

    if (isParametrized)
        m_inParametrizedBranch = true;

    readTypeAttributes(*type);
    readTypeContents(name, *type);

    return type;
}

void AstXmlParser::readTypeAttributes(Types::Type &type)
{
    TypeAttributesAssigningVisitor visitor(m_xmlReader.attributes());
    type.accept(visitor);
}

std::unique_ptr<Types::Type> AstXmlParser::createReferenceType(
    const SourceLocation &location)
{
    const QString refName = readTypeAssignmentAttribute();
    QString module = readModuleAttribute();
    if (module.isEmpty()) {
        module = m_currentModule;
    }

    const TypeAssignment *referencedType = m_currentDefinitions->type(refName);

    if (!m_inParametrizedBranch) {
        auto ref = std::make_unique<TypeReference>(refName, module, location);
        m_data[m_currentFile]->addTypeReference(std::move(ref));
    }

    return std::make_unique<Types::UserdefinedType>(refName, module, referencedType);
}

void AstXmlParser::readReferredTypeDetails(Types::Type &type)
{
    auto referedType = readType();
    ChildItemAddingVisitor visitor(m_xmlReader.attributes(), m_currentFile, std::move(referedType));
    type.accept(visitor);
}

void AstXmlParser::readAcnArguments(Types::Type &type)
{
    AcnArgumentPtrs arguments;
    while (skipToChildElement("argument"))
        arguments.push_back(std::make_unique<AcnArgument>(m_xmlReader.readElementText()));

    AcnDefinedItemsAddingVisitor visitor(std::move(arguments));
    type.accept(visitor);
}

void AstXmlParser::readTypeContents(const QString &name, Types::Type &type)
{
    if (name == QStringLiteral("SequenceType")) {
        readSequenceAsn(type);
    } else if (name == QStringLiteral("SEQUENCE")) {
        readSequence(type);
    } else if (name == QStringLiteral("SEQUENCE_OF") || name == QStringLiteral("SEQUENCE OF") || name == QStringLiteral("SequenceOfType")) {
        readSequenceOf(type);
    } else if (m_xmlReader.name() == "ChoiceType") {
        readChoiceAsn(type);
    } else if (name == QStringLiteral("CHOICE")) {
        readChoice(type);
    } else if (name == QStringLiteral("REFERENCE_TYPE") || name == QStringLiteral("REFERENCE TYPE") || name == QStringLiteral("ReferenceType")) {
        readReferenceType(type);
    } else if (name == QStringLiteral("INTEGER") || name.startsWith(QStringLiteral("Integer"), Qt::CaseInsensitive)) {
        readInteger(type);
    } else if (name == QStringLiteral("REAL") || name.startsWith(QStringLiteral("Real"), Qt::CaseInsensitive)) {
        readReal(type);
    } else if (name == QStringLiteral("ENUMERATED") || name.startsWith(QStringLiteral("Enumerated"), Qt::CaseInsensitive)) {
        readEnumerated(type);
    } else if (name == QStringLiteral("OCTET_STRING") || name == QStringLiteral("OCTET STRING") || name == QStringLiteral("OctetStringType")) {
        readOctetString(type);
    } else if (name == QStringLiteral("IA5String") || name == QStringLiteral("IA5String")) {
        readIA5String(type);
    } else if (name == QStringLiteral("NumericString")) {
        readNumericString(type);
    } else if (name == QStringLiteral("BIT_STRING") || name == QStringLiteral("BIT STRING") || name == QStringLiteral("BitStringType")) {
        readBitString(type);
    } else {
        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readSequence(Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("SEQUENCE_COMPONENT"))
            readSequenceComponent(type);
        else if (m_xmlReader.name() == QStringLiteral("ACN_COMPONENT"))
            readAcnComponent(type);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readSequenceComponent(Types::Type &type)
{
    auto attributes = m_xmlReader.attributes();
    auto childType = findAndReadType();

    ChildItemAddingVisitor visitor(attributes, m_currentFile, std::move(childType));
    type.accept(visitor);
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readAcnComponent(Types::Type &type)
{
    auto alignToNext = readIsAlignedToNext();
    auto name = readNameAttribute();
    auto id = readIdAttribute(QStringLiteral("Id"));
    auto component = readTypeDetails(readTypeAttribute(), {}, false, alignToNext);

    AcnDefinedItemsAddingVisitor visitor(
        std::make_unique<AcnSequenceComponent>(id, name, std::move(component)));
    type.accept(visitor);
}

void AstXmlParser::readSequenceAsn(Types::Type &type)
{
    auto sequence = dynamic_cast<Types::Sequence *>(&type);
    if (!sequence) {
        return;
    }

    while (skipToChildElement(QStringLiteral("SequenceOrSetChild"))) {
        const QString name = readVarNameAttribute();
        auto sequenceType = findAndReadType();
        sequenceType->setIdentifier(name);
        sequence->addChild(std::move(sequenceType));
        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readSequenceOf(Types::Type &type)
{
    parseRange<qlonglong>(type);

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readConstraints(type);
        else if (m_xmlReader.name() == QStringLiteral("Asn1Type")) {
            auto attributes = m_xmlReader.attributes();
            auto childType = readType();

            ChildItemAddingVisitor visitor(attributes, m_currentFile, std::move(childType));
            type.accept(visitor);
        } else if (m_xmlReader.name() == QStringLiteral("Type")) {
            auto childType = readType();
            if (childType) {
                type.addChild(std::move(childType));
            }
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void AstXmlParser::readChoiceAsn(Types::Type &type)
{
    auto choice = dynamic_cast<Types::Choice *>(&type);
    if (!choice) {
        return;
    }

    while (skipToChildElement(QStringLiteral("ChoiceChild"))) {
        auto name = readVarNameAttribute();
        auto choiceType = findAndReadType();
        choiceType->setIdentifier(name);

        choice->addChild(std::move(choiceType));

        if (m_xmlReader.name() != QStringLiteral("ChoiceChild")) {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void AstXmlParser::readChoice(Types::Type &type)
{
    while (skipToChildElement(QStringLiteral("CHOICE_ALTERNATIVE"))) {
        auto attributes = m_xmlReader.attributes();
        auto childType = findAndReadType();

        ChildItemAddingVisitor visitor(attributes, m_currentFile, std::move(childType));
        type.accept(visitor);

        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readInteger(Types::Type &type)
{
    parseRange<qlonglong>(type);
    findAndReadConstraints(type);
}

void AstXmlParser::readReal(Types::Type &type)
{
    parseRange<double>(type);
    findAndReadConstraints(type);
}

void AstXmlParser::readEnumerated(Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Items"))
            readEnumeratedItem(type);
        else if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readConstraints(type);
        else if (m_xmlReader.name() == QStringLiteral("EnumValues"))
            readEnumValues(type);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readOctetString(Types::Type &type)
{
    findAndReadConstraints(type);
}

void AstXmlParser::readIA5String(Types::Type &type)
{
    findAndReadConstraints(type);
}

void AstXmlParser::readNumericString(Types::Type &type)
{
    findAndReadConstraints(type);
}

void AstXmlParser::readBitString(Types::Type &type)
{
    findAndReadConstraints(type);
}

void AstXmlParser::readEnumeratedItem(Types::Type &type)
{
    int index = 0;
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Item")) {
            EnumeratedItemAddingVisitor visitor(m_xmlReader.attributes(), m_currentFile, index++);
            type.accept(visitor);
        }

        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readEnumValues(Types::Type &type)
{
    QStringList values;

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("EnumValue")) {
            const QString value = m_xmlReader.attributes().value("StringValue").toString();
            if (!value.isEmpty()) {
                values.append(value);
            }
        }

        m_xmlReader.skipCurrentElement();
    }

    type.m_parameters[ASN1_VALUES] = values;
}

void AstXmlParser::readReferenceType(Types::Type &type)
{
    const TypeAssignment *assignment = m_currentDefinitions->type(type.typeName());
    if (assignment) {
        if (type.parameters().isEmpty()) {
            type.setParameters(assignment->type()->parameters());
        }
    } 

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("AcnArguments"))
            readAcnArguments(type);
        else if (m_xmlReader.name() == QStringLiteral("Asn1Type") || m_xmlReader.name() == QStringLiteral("Type"))
            readReferredTypeDetails(type);
        else
            m_xmlReader.skipCurrentElement();
    }
}

namespace {
class ConstraintReadingVisitor : public Types::TypeMutatingVisitor
{
public:
    ConstraintReadingVisitor(QXmlStreamReader &xmlReader)
        : m_xmlReader(xmlReader)
    {}

    void visit(Types::Boolean &type) override { readConstraints<BooleanValue>(type); }

    void visit(Types::Null &type) override { Q_UNUSED(type); }

    void visit(Types::BitString &type) override
    {
        readConstraints<BitStringValue>(type);
    }

    void visit(Types::OctetString &type) override
    {
        readConstraints<OctetStringValue>(type);
    }

    void visit(Types::IA5String &type) override { readConstraints<StringValue>(type); }

    void visit(Types::NumericString &type) override
    {
        readConstraints<StringValue>(type);
    }

    void visit(Types::Enumerated &type) override { readConstraints<EnumValue>(type); }

    void visit(Types::Choice &type) override { Q_UNUSED(type); }

    void visit(Types::Sequence &type) override { Q_UNUSED(type); }

    void visit(Types::SequenceOf &type) override
    {
        // any value constraint would be valid, only size matters
        readConstraints<IntegerValue>(type);
    }

    void visit(Types::Real &type) override { readConstraints<RealValue>(type); }

    void visit(Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Types::Integer &type) override { readConstraints<IntegerValue>(type); }

    void visit(Types::UserdefinedType &type) override { Q_UNUSED(type); }

private:
    template<typename T>
    void readConstraints(Constraints::WithConstraints<T> &type) const
    {
        AstXmlConstraintParser<T> parser(m_xmlReader, type.constraints());
        parser.parse();
    }

    QXmlStreamReader &m_xmlReader;
};
} // namespace

void AstXmlParser::findAndReadConstraints(Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readConstraints(type);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readConstraints(Types::Type &type)
{
    ConstraintReadingVisitor visitor(m_xmlReader);
    type.accept(visitor);
}

bool AstXmlParser::skipToChildElement(const QString &name)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == name)
            return true;
        else
            m_xmlReader.skipCurrentElement();
    }

    return false;
}

bool AstXmlParser::skipToChildElement(const QStringList &names)
{
    while (m_xmlReader.readNextStartElement()) {
        if (names.contains(m_xmlReader.name().toString()))
            return true;
        else
            m_xmlReader.skipCurrentElement();
    }

    return false;
}

template<typename T>
void AstXmlParser::parseRange(Types::Type &type)
{
    QXmlStreamAttributes attributes = m_xmlReader.attributes();

    auto parseAttribute = [&](const QString &attrName, const QString &mapName) {
        if (attributes.hasAttribute(attrName)) {
            bool ok;
            const QString valueText = attributes.value(attrName).toString();
            if (std::is_same<T, qlonglong>::value) {
                qlonglong valueInt = valueText.toLongLong(&ok);
                if (ok && !type.parameters().contains(mapName))
                    type.m_parameters[mapName] = valueInt;
            } else {
                double valueDouble = valueText.toDouble(&ok);
                if (ok && !type.parameters().contains(mapName))
                    type.m_parameters[mapName] = static_cast<T>(valueDouble);
            }
        }
    };

    parseAttribute("Min", ASN1_MIN);
    parseAttribute("Max", ASN1_MAX);
}
