#include <QDomDocument>
#include <QDomElement>
#include <QFile>

#include "xmlparser.h"

XMLParser::XMLParser(const QString &file, QObject *parent) :
    QObject(parent),
    m_fileName(file)
{

}

QVariantMap XMLParser::parseAsn1Xml()
{
    QVariantMap result;

    QDomDocument doc;
    QDomElement root;

    if (m_fileName.isEmpty())
        return result;

    QFile f(m_fileName);
    if (f.open( QIODevice::ReadOnly )) {
        QString errorMsg;

        bool ok = doc.setContent(f.readAll(), &errorMsg);
        f.close();
        if (!ok) {
            Q_EMIT parseError(errorMsg);
            return result;
        }
    }
    else {
        Q_EMIT parseError(f.errorString());
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

    m_fileName = root.attribute("FileName");

    QDomNodeList asn1Modules = root.elementsByTagName("Asn1Module");
    for(int x = 0; x < asn1Modules.size(); ++x) {
        QDomElement asn1Module = asn1Modules.at(x).toElement();

        QString moduleID = asn1Module.attribute("ID");
        QDomNodeList typeAssignments = asn1Module.firstChildElement("TypeAssignments")
                                                 .elementsByTagName("TypeAssignment");

        for(int x = 0; x < typeAssignments.size(); ++x) {
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

    result["id"]            = name;
    result["isOptional"]    = false;
    result["alwaysPresent"] = true;
    result["alwaysAbsent"]  = false;

    const QDomElement typeElem = type.firstChild().toElement();
    const QString typeName = typeElem.tagName();

    if (typeName == "IntegerType") {
        result["type"] = "integer";
        result["min"]  = typeElem.attribute("Min").toInt();
        result["max"]  = typeElem.attribute("Max").toInt();
    }
    else if (typeName == "RealType") {
        result["type"] = "double";
        result["min"]  = typeElem.attribute("Min").toDouble();
        result["max"]  = typeElem.attribute("Max").toDouble();
    }
    else if (typeName == "BooleanType") {
        result["type"]    = "bool";
        result["default"] = false;
    }
    else if (typeName == "SequenceType") {
        result["type"] = "sequence";
        parseSequenceType(typeElem, result);
    }
    else if (typeName == "SequenceOfType") {
        result["type"]      = "seqof";
        result["min"]       = typeElem.attribute("Min").toInt();
        result["max"]       = typeElem.attribute("Max").toInt();
        result["seqoftype"] = parseType(typeElem.firstChild().toElement());
    }
    else if (typeName == "EnumeratedType") {
        result["type"] = "enumerated";
        parseEnumeratedType(typeElem, result);
    }
    else if (typeName == "ChoiceType") {
        result["type"] = "choice";
        parseChoiceType(typeElem, result);
    }
    else if (typeName.endsWith("StringType")) {
        result["type"] = "string";
        result["min"]  = typeElem.attribute("Min").toInt();
        result["max"]  = typeElem.attribute("Max").toInt();
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
    result["children"] = QVariantList();

    for(QDomNode n = type.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement elem = n.toElement();

        QVariantMap childType = parseType(elem.firstChildElement("Type"),
                                          elem.attribute("VarName"));


        childType["isOptional"]    = elem.attribute("Optional") == "True";
        childType["alwaysPresent"] = elem.attribute("alwaysPresent") == "True";
        childType["alwaysAbsent"]  = elem.attribute("alwaysAbsent") == "False";

        result["children"].toList().append(childType);
    }
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

    result["values"]    = QVariantList();
    result["valuesInt"] = QVariantList();

    for(int x = 0; x < enumValueList.size(); ++x) {
        QDomElement enumValue = enumValueList.at(x).toElement();

        result["values"].toList().append(enumValue.attribute("StringValue"));
        result["valuesInt"].toList().append(enumValue.attribute("IntValue"));
    }
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

    result["choices"]   = QVariantList();
    result["choiceIdx"] = QVariantList();

    for(QDomNode n = type.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement elem = n.toElement();

        result["choices"].toList().append(parseType(elem.firstChildElement("Type"),
                                                    elem.attribute("VarName")));
        result["choiceIdx"].toList().append(elem.attribute("EnumID"));
    }
}


