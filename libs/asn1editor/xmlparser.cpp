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

QVariantList XMLParser::parseAsn1XmlFile(const QString &filename)
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

    return QVariantList();
}

QVariantList XMLParser::parseAsn1XmlContent(const QString &content)
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
        typeAssignments.append(asn1Module.firstChildElement("TypeAssignments")
                                       .elementsByTagName("TypeAssignment"));
    }

    for (const QDomNodeList &typeAssignment : typeAssignments) {
        for (int x = 0; x < typeAssignment.size(); ++x) {
            QDomElement elem = typeAssignment.at(x).toElement();

            asn1TypesData.append(parseType(typeAssignments,
                                           elem.firstChildElement("Type"),
                                           elem.attribute("Name")));
        }
    }

    return asn1TypesData;
}

QVariantMap XMLParser::parseType(const QList<QDomNodeList> &typeAssignments,
                                 const QDomElement &type,
                                 const QString &name)
{
    QVariantMap typeData;

    typeData["name"] = name;
    typeData["isOptional"] = false;
    typeData["alwaysPresent"] = true;
    typeData["alwaysAbsent"] = false;

    QDomElement typeElem = type.firstChild().toElement();
    QString typeName = typeElem.tagName();

    // find type node for ReferenceType
    while (typeName == "ReferenceType") {
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

    if (typeName == "IntegerType") {
        typeData["type"] = "integer";
        typeData["min"] = typeElem.attribute("Min").toInt();
        typeData["max"] = typeElem.attribute("Max").toInt();
    } else if (typeName == "RealType") {
        typeData["type"] = "double";
        typeData["min"] = typeElem.attribute("Min").toDouble();
        typeData["max"] = typeElem.attribute("Max").toDouble();
    } else if (typeName == "BooleanType") {
        typeData["type"] = "bool";
        typeData["default"] = false;
    } else if (typeName == "SequenceType") {
        typeData["type"] = "sequence";
        parseSequenceType(typeAssignments, typeElem, typeData);
    } else if (typeName == "SequenceOfType") {
        typeData["type"] = "sequenceOf";
        typeData["min"] = typeElem.attribute("Min").toInt();
        typeData["max"] = typeElem.attribute("Max").toInt();
        typeData["seqoftype"] = parseType(typeAssignments, typeElem.firstChild().toElement());
    } else if (typeName == "EnumeratedType") {
        typeData["type"] = "enumerated";
        parseEnumeratedType(typeElem, typeData);
    } else if (typeName == "ChoiceType") {
        typeData["type"] = "choice";
        parseChoiceType(typeAssignments, typeElem, typeData);
    } else if (typeName.endsWith("StringType")) {
        typeData["type"] = "string";
        typeData["min"] = typeElem.attribute("Min").toInt();
        typeData["max"] = typeElem.attribute("Max").toInt();
    }

    return typeData;
}

void XMLParser::parseSequenceType(const QList<QDomNodeList> &typeAssignments,
                                  const QDomElement &type,
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

        QVariantMap childType = parseType(typeAssignments,
                                          elem.firstChildElement("Type"),
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

void XMLParser::parseChoiceType(const QList<QDomNodeList> &typeAssignments,
                                const QDomElement &type,
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
    QVariantList choiceIdx;

    for (QDomNode n = type.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement elem = n.toElement();

        choices.append(parseType(typeAssignments,
                                 elem.firstChildElement("Type"),
                                 elem.attribute("VarName")));
        choiceIdx.append(elem.attribute("EnumID"));
    }

    result["choices"] = choices;
    result["choiceIdx"] = choiceIdx;
}

} // namespace asn1
