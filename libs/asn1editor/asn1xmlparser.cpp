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

#include "asn1xmlparser.h"

#include "asn1const.h"

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include <QProcess>
#include <QVariantMap>

const QString asn1Command = "asn1 -ast %1 %2"; // TODO: test for win and linux

namespace asn1 {

Asn1XMLParser::Asn1XMLParser(QObject *parent)
    : QObject(parent)
{
}

QVariantList Asn1XMLParser::parseAsn1File(const QString &filePath, const QString &fileName)
{
    // convert ASN.1 to XML
    QProcess asn1Process;

    auto fullFilePath = [](const QString &path, const QString &name) {
        return QFileInfo(QDir(path), name).absoluteFilePath();
    };

    QString asn1FileName = fullFilePath(filePath, fileName);
    QString asn1XMLFileName = fullFilePath(QDir::tempPath(), "asn1.xml");

    connect(&asn1Process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [&](int, QProcess::ExitStatus exitStatus) {
                if (exitStatus == QProcess::CrashExit)
                    qWarning() << "Process was crashed";
            });

    connect(&asn1Process, &QProcess::errorOccurred,
            [&](QProcess::ProcessError) { qWarning() << asn1Process.errorString(); });

    asn1Process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    asn1Process.setProcessChannelMode(QProcess::MergedChannels);
    asn1Process.start(QString(asn1Command).arg(asn1XMLFileName, asn1FileName));
    asn1Process.waitForFinished();

    auto error = asn1Process.readAll();
    if (!error.isEmpty()) {
        qWarning() << error;
        return {};
    }

    QVariantList asn1TypesData = parseAsn1XmlFile(asn1XMLFileName);

    QFile::remove(asn1XMLFileName);

    return asn1TypesData;
}

QVariantList Asn1XMLParser::parseAsn1XmlFile(const QString &fileName)
{
    if (QFileInfo::exists(fileName)) {
        QFile file(fileName);

        if (file.open(QIODevice::ReadOnly)) {
            const auto content = file.readAll();
            file.close();

            return parseXml(content);
        } else
            Q_EMIT parseError(file.errorString());
    } else
        Q_EMIT parseError(tr("File not found"));

    return QVariantList();
}

QVariantList Asn1XMLParser::parseXml(const QString &content)
{
    QDomDocument doc;
    QDomElement root;
    QString errorMsg;
    QVariantList asn1TypesData;

    if (!doc.setContent(content, &errorMsg)) {
        Q_EMIT parseError(errorMsg);
        return asn1TypesData;
    }

    root = doc.documentElement();
    if (root.isNull() || root.tagName() != "ASN1AST") {
        Q_EMIT parseError(tr("Invalid XML format"));
        return asn1TypesData;
    }

    root = root.firstChildElement("Asn1File");
    if (root.isNull()) {
        Q_EMIT parseError(tr("Invalid XML format"));
        return asn1TypesData;
    }

    QDomNodeList asn1Modules = root.elementsByTagName("Asn1Module");
    QList<QDomNodeList> typeAssignments;

    for (int x = 0; x < asn1Modules.size(); ++x) {
        QDomElement asn1Module = asn1Modules.at(x).toElement();

        // store all TypeAssignment nodes
        typeAssignments.append(asn1Module.firstChildElement("TypeAssignments").elementsByTagName("TypeAssignment"));
    }

    for (const QDomNodeList &typeAssignment : typeAssignments) {
        for (int x = 0; x < typeAssignment.size(); ++x) {
            QDomElement elem = typeAssignment.at(x).toElement();

            asn1TypesData.append(parseType(typeAssignments, elem.firstChildElement("Type"), elem.attribute("Name")));
        }
    }

    return asn1TypesData;
}

QVariantMap Asn1XMLParser::parseType(const QList<QDomNodeList> &typeAssignments, const QDomElement &type,
                                     const QString &name)
{
    QVariantMap typeData;

    typeData[ASN1_NAME] = name;
    typeData[ASN1_IS_OPTIONAL] = false;

    QDomElement typeElem = type.firstChild().toElement();
    QString typeName = typeElem.tagName();

    auto typeByTypeName = [](const QString &typeName) {
        static QMap<QString, ASN1Type> asn1TypeStringMap{
            { "IntegerType", ASN1Type::INTEGER },       { "RealType", ASN1Type::DOUBLE },
            { "BooleanType", ASN1Type::BOOL },          { "SequenceType", ASN1Type::SEQUENCE },
            { "SequenceOfType", ASN1Type::SEQUENCEOF }, { "EnumeratedType", ASN1Type::ENUMERATED },
            { "ChoiceType", ASN1Type::CHOICE }
        };

        return asn1TypeStringMap.contains(typeName) ? asn1TypeStringMap[typeName] : ASN1Type::STRING;
    };

    // find type node for ReferenceType
    while (typeName == "ReferenceType") {
        parseRange<int>(typeElem, typeData);

        for (const QDomNodeList &typeAssignment : typeAssignments) {
            for (int x = 0; x < typeAssignment.size(); ++x) {
                QDomElement elem = typeAssignment.at(x).toElement();
                if (elem.attribute("Name") == typeElem.attribute("ReferencedTypeName")) {
                    typeElem = elem.firstChildElement("Type").firstChild().toElement();
                    typeName = typeElem.tagName();

                    break;
                }
            }

            if (typeName != "ReferenceType")
                break;
        }
    }

    typeData[ASN1_TYPE] = typeByTypeName(typeName);

    switch (static_cast<ASN1Type>(typeData[ASN1_TYPE].toInt())) {
    case INTEGER:
    case STRING:
        parseRange<int>(typeElem, typeData);
        break;
    case DOUBLE:
        parseRange<double>(typeElem, typeData);
        break;
    case BOOL:
        typeData["default"] = false;
        break;
    case SEQUENCE:
        parseSequenceType(typeAssignments, typeElem, typeData);
        break;
    case SEQUENCEOF:
        typeData[ASN1_SEQOFTYPE] = parseType(typeAssignments, typeElem.firstChild().toElement());
        parseRange<int>(typeElem, typeData);
        break;
    case ENUMERATED:
        parseEnumeratedType(typeElem, typeData);
        break;
    case CHOICE:
        parseChoiceType(typeAssignments, typeElem, typeData);
        break;
    }

    return typeData;
}

void Asn1XMLParser::parseSequenceType(const QList<QDomNodeList> &typeAssignments, const QDomElement &type,
                                      QVariantMap &result)
{
    /*
<SequenceType>
    <SequenceOrSetChild VarName="foo" Optional="False" Line="8" CharPositionInLine="21">
        <Type Line="8" CharPositionInLine="25">
            <BooleanType />
        </Type>
    </SequenceOrSetChild>
    <SequenceOrSetChild VarName="bar" Optional="True" Line="8" CharPositionInLine="34">
        ...
    </SequenceOrSetChild>
 </SequenceType>
*/
    QVariantList children;

    for (QDomNode n = type.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement elem = n.toElement();

        QVariantMap childType = parseType(typeAssignments, elem.firstChildElement("Type"), elem.attribute("VarName"));

        childType[ASN1_IS_OPTIONAL] = elem.attribute("Optional") == "True";

        children.append(childType);
    }

    result[ASN1_CHILDREN] = children;
}

void Asn1XMLParser::parseEnumeratedType(const QDomElement &type, QVariantMap &result)
{
    /*
<EnumeratedType Extensible="False" ValuesAutoCalculated="False">
    <EnumValues>
        <EnumValue StringValue="red" IntValue="0" Line="17" CharPositionInLine="4" EnumID ="red" />
        ...
    </EnumValues>
</EnumeratedType>
 */

    // get all EnumValue elements
    QDomNodeList enumValueList = type.firstChildElement().elementsByTagName("EnumValue");

    QVariantList values;

    for (int x = 0; x < enumValueList.size(); ++x) {
        QDomElement enumValue = enumValueList.at(x).toElement();

        values.append(enumValue.attribute("StringValue"));
    }

    result[ASN1_VALUES] = values;
}

void Asn1XMLParser::parseChoiceType(const QList<QDomNodeList> &typeAssignments, const QDomElement &type,
                                    QVariantMap &result)
{
    /*
<ChoiceType>
    <ChoiceChild VarName="x" Line="20" CharPositionInLine="15" EnumID ="x_PRESENT">
        <Type Line="20" CharPositionInLine="17">
            <BooleanType />
        </Type>
    </ChoiceChild>
    <ChoiceChild VarName="y" Line="20" CharPositionInLine="26" EnumID ="y_PRESENT">
        ...
    </ChoiceChild>
</ChoiceType>
*/

    QVariantList choices;

    for (QDomNode n = type.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement elem = n.toElement();

        choices.append(parseType(typeAssignments, elem.firstChildElement("Type"), elem.attribute("VarName")));
    }

    result[ASN1_CHOICES] = choices;
}

template<typename T>
void Asn1XMLParser::parseRange(const QDomElement &type, QVariantMap &result)
{
    bool ok;
    double value;

    auto parseAttribute = [&](const QString &attrName, const QString &mapName) {
        if (type.hasAttribute(attrName)) {
            value = type.attribute(attrName).toDouble(&ok);
            if (ok && !result.contains(mapName))
                result[mapName] = static_cast<T>(value);
        }
    };

    parseAttribute("Min", ASN1_MIN);
    parseAttribute("Max", ASN1_MAX);
}

} // namespace asn1
