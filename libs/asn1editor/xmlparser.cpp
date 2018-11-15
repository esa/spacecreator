/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include <QVariantMap>

#include "xmlparser.h"

namespace asn1 {

XMLParser::XMLParser(QObject *parent)
    : QObject(parent)
{
}

QVariantMap XMLParser::parseAsn1XmlFile(const QString &filename)
{
    if (QFileInfo::exists(filename)) {
        QFile file(filename);

        if (file.open(QIODevice::ReadOnly)) {
            const auto content = file.readAll();
            file.close();

            return parseAsn1XmlContent(content);
        } else
            Q_EMIT parseError(file.errorString());
    } else
        Q_EMIT parseError(tr("File not found"));

    return QVariantMap();
}

QVariantMap XMLParser::parseAsn1XmlContent(const QString &content)
{
    QDomDocument doc;
    QDomElement root;
    QString errorMsg;
    QVariantMap result;

    if (!doc.setContent(content, &errorMsg)) {
        Q_EMIT parseError(errorMsg);
        return result;
    }

    root = doc.documentElement();
    if (root.isNull() || root.tagName() != "ASN1AST") {
        Q_EMIT parseError(tr("Invalid XML format"));
        return result;
    }

    root = root.firstChildElement("Asn1File");
    if (root.isNull()) {
        Q_EMIT parseError(tr("Invalid XML format"));
        return result;
    }

    QDomNodeList asn1Modules = root.elementsByTagName("Asn1Module");
    for (int x = 0; x < asn1Modules.size(); ++x) {
        QDomElement asn1Module = asn1Modules.at(x).toElement();

        QString moduleID = asn1Module.attribute("ID");
        QDomNodeList typeAssignments = asn1Module.firstChildElement("TypeAssignments")
                                               .elementsByTagName("TypeAssignment");

        for (int x = 0; x < typeAssignments.size(); ++x) {
            QDomElement elem = typeAssignments.at(x).toElement();

            result[moduleID + ":" + elem.attribute("Name")] = parseType(elem.firstChildElement("Type"),
                                                                        elem.attribute("Name"));
        }
    }

    return result;
}

QVariantMap XMLParser::parseType(const QDomElement &type, const QString &name)
{
    QVariantMap result;

    result["name"] = name;
    result["isOptional"] = false;
    result["alwaysPresent"] = true;
    result["alwaysAbsent"] = false;

    const QDomElement typeElem = type.firstChild().toElement();
    const QString typeName = typeElem.tagName();

    if (typeName == "IntegerType") {
        result["type"] = "integer";
        result["min"] = typeElem.attribute("Min").toInt();
        result["max"] = typeElem.attribute("Max").toInt();
    } else if (typeName == "RealType") {
        result["type"] = "double";
        result["min"] = typeElem.attribute("Min").toDouble();
        result["max"] = typeElem.attribute("Max").toDouble();
    } else if (typeName == "BooleanType") {
        result["type"] = "bool";
        result["default"] = false;
    } else if (typeName == "SequenceType") {
        result["type"] = "sequence";
        parseSequenceType(typeElem, result);
    } else if (typeName == "SequenceOfType") {
        result["type"] = "sequenceOf";
        result["min"] = typeElem.attribute("Min").toInt();
        result["max"] = typeElem.attribute("Max").toInt();
        result["seqoftype"] = parseType(typeElem.firstChild().toElement());
    } else if (typeName == "EnumeratedType") {
        result["type"] = "enumerated";
        parseEnumeratedType(typeElem, result);
    } else if (typeName == "ChoiceType") {
        result["type"] = "choice";
        parseChoiceType(typeElem, result);
    } else if (typeName.endsWith("StringType")) {
        result["type"] = "string";
        result["min"] = typeElem.attribute("Min").toInt();
        result["max"] = typeElem.attribute("Max").toInt();
    }

    return result;
}

void XMLParser::parseSequenceType(const QDomElement &type, QVariantMap &result)
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

        QVariantMap childType = parseType(elem.firstChildElement("Type"),
                                          elem.attribute("VarName"));

        childType["isOptional"] = elem.attribute("Optional") == "True";
        childType["alwaysPresent"] = elem.attribute("alwaysPresent") == "True";
        childType["alwaysAbsent"] = elem.attribute("alwaysAbsent") == "False";

        children.append(childType);
    }

    result["children"] = children;
}

void XMLParser::parseEnumeratedType(const QDomElement &type, QVariantMap &result)
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
    QVariantList valuesInt;

    for (int x = 0; x < enumValueList.size(); ++x) {
        QDomElement enumValue = enumValueList.at(x).toElement();

        values.append(enumValue.attribute("StringValue"));
        valuesInt.append(enumValue.attribute("IntValue"));
    }

    result["values"] = values;
    result["valuesInt"] = valuesInt;
}

void XMLParser::parseChoiceType(const QDomElement &type, QVariantMap &result)
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
    QVariantList choiceIdx;

    for (QDomNode n = type.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement elem = n.toElement();

        choices.append(parseType(elem.firstChildElement("Type"),
                                 elem.attribute("VarName")));
        choiceIdx.append(elem.attribute("EnumID"));
    }

    result["choices"] = choices;
    result["choiceIdx"] = choiceIdx;
}

} // namespace asn1
